#pragma once

#include <utility>
#include <vector>
#include "TableRow.hpp"
#include "../utils/timer.hpp"
#include <thread>

namespace csv {
    // TODO: remove split function
    // Move row parsing into 1 function
    auto split_string( const std::string& input, char split_by, int32_t reserve = -1 ) -> std::vector< std::string > {
        std::string current;
//        current.reserve( 512 );
        std::vector< std::string > lines;
        if ( reserve > 0 ) lines.reserve( reserve );

        for ( const auto c: input ) {
            if ( c == split_by ) {
                lines.emplace_back( current );
                current.clear( );
            } else {
                current += c;
//                current.push_back( c );
            }
        }

//        auto last_current = current.str( );
        if ( !current.empty( ) ) lines.emplace_back( current );

//        std::cout << "done" << std::endl;
        return lines;
    }

    auto trim( std::string& str ) -> std::string {
        str.erase( str.find_last_not_of( ' ' ) + 1 );         //suffixing spaces
        str.erase( 0, str.find_first_not_of( ' ' ) );       //prefixing spaces
        return str;
    }

    class Table {
    public:
        Table( ) { }

        Table( const std::vector< std::string >& headings, std::vector< TableRow > rows,
               std::vector< int32_t > longest_items )
                : m_headings(
                ( headings )
        ),
                  m_longest_input_per_column( std::move( longest_items ) ),
                  m_rows( std::move( rows ) ) { }

        static auto parse( const std::string& content, char delimiter ) -> std::shared_ptr< Table > {
            utils::Timer parse_timer( "parse" );
            auto lines = split_string( content, '\n' );
            auto headings = split_string( lines[ 0 ], delimiter );
            const auto headings_size = headings.size( );
            std::vector< int32_t > longest_items;
            longest_items.reserve( headings_size );

            for ( auto& heading: headings ) {
                heading = trim( heading );
                longest_items.push_back( 0 );
            }

            std::vector< TableRow > rows;
            {
                utils::Timer timer( "reserve" );
                rows.reserve( lines.size( ) );
            }
            {
                utils::Timer timer( "fill" );
                for ( int i = 0; i < lines.size( ); ++i ) {
                    rows.emplace_back( );
                }
            }


            auto line_amount = lines.size( );

            constexpr auto amount_threads = 12;

            const auto add_run_thread = [ & ]( int offset ) -> void {
                utils::Timer timer( std::to_string( offset ) );
                for ( int i = offset; i < line_amount; i += amount_threads ) {
                    if ( lines.size( ) < i ) return;
                    auto parsed = parse_row( lines[ i ], delimiter, static_cast<int32_t>(headings_size),
                                             longest_items );
                    if ( !parsed ) continue;
                    rows[ i ] = *parsed;
                }
            };

            {
                utils::Timer timer( "wait for all" );
                std::vector< std::thread > threads;
                threads.reserve( amount_threads );
                for ( int i = 0; i < amount_threads; ++i ) {
                    threads.emplace_back( add_run_thread, i + 1 );
                }

                for ( auto& thread: threads ) {
                    thread.join( );
                }
            }

//            for ( int i = 1; i < lines.size( ); ++i ) {
//                std::thread( [ &, i ]( ) -> void {
//                    auto parsed = parse_row( lines[ i ], delimiter, static_cast<int32_t>(headings_size),
//                                             longest_items );
//                    if ( !parsed ) return;
//                    rows[ i ] = *parsed;
//                } ).detach( );
//            }

            return std::make_shared< Table >( headings, rows, longest_items );
        }

        auto encode( char delimiter ) -> std::string {
            utils::Timer encode_timer( "encode" );
            std::string out;

//            out.resize( 50000 );

            {
                utils::Timer timer( "headings encode" );
                out += m_headings.encode( delimiter, m_longest_input_per_column ) + "\n";
            }

            constexpr auto amount_threads = 4;

            static std::array< std::string, amount_threads > output{ };
            static const auto rows_size = m_rows.size( );
            static auto rows = m_rows;
            static auto longest_input_per_column = m_longest_input_per_column;

            const auto encode_rows = [ delimiter ](
                    int32_t offset ) -> void {
//                utils::Timer timer( "encode rows" );
                std::string s;
//                s.resize( ( 256 / amount_threads ) * rows_size );
                for ( int i = offset; i < rows_size; i += amount_threads ) {
                    if ( i > rows_size ) return;
                    s += rows[ i ].encode( delimiter, longest_input_per_column ) + "\n";
                }

                output[ offset ] = std::move( s );
            };

            std::vector< std::thread > threads;
            threads.reserve( amount_threads );
            for ( int i = 0; i < amount_threads; ++i ) {
                threads.emplace_back( encode_rows, i );
            }

            {
                utils::Timer timer( "thread join" );
                for ( auto& thread: threads ) {
                    thread.join( );
                }
            }

            {
                utils::Timer timer( "join output" );
//                out.resize( output[ 0 ].size( ) * amount_threads );
                for ( int i = 0; i < amount_threads; ++i ) {
                    out += output[ i ];
                }
            }

//            for ( const auto& row: m_rows ) {
//                ss << row.encode( delimiter, m_longest_input_per_column ) << "\n";
//            }

            return out;
        }

    private:
        static auto parse_row( const std::string& line, char delimiter, int32_t headings_size,
                               std::vector< int32_t >& longest_items ) -> std::optional< TableRow > {
            auto columns = split_string( line,
                                         delimiter,
                                         headings_size );

            if ( columns.empty( ) ) return std::nullopt;

            for ( int j = 0; j < columns.size( ); ++j ) {
                columns[ j ] = trim( columns[ j ] );

                if ( longest_items[ j ] < columns[ j ].size( ) ) longest_items[ j ] = columns[ j ].size( );
            }

            while ( columns.size( ) < headings_size ) columns.emplace_back( "" );

            return TableRow( columns );
        }

    private:
        TableRow m_headings{ };
        std::vector< int32_t > m_longest_input_per_column{ };
        std::vector< TableRow > m_rows{ };
    };
}