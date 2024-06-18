#include "Table.hpp"

#include <thread>

#include "../utils/string.hpp"

namespace csv {
    auto Table::parse( const std::string& content, const char delimiter ) -> std::shared_ptr< Table > {
        auto timer = utils::Timer::start( "Table::parse" );

        const auto lines = utils::string::split( content, '\n' );
        auto headings = utils::string::split( lines[ 0 ], delimiter );
        const auto headings_size = headings.size( );

        std::vector< int32_t > longest_items;
        longest_items.reserve( headings_size );

        for ( auto& heading : headings ) {
            heading = utils::string::trim( heading );
            longest_items.push_back( static_cast< int32_t >( heading.size( ) ) );
        }

        std::vector< TableRow > rows;
        rows.reserve( lines.size( ) );

        for ( int i = 0; i < lines.size( ); ++i ) {
            rows.emplace_back( );
        }


        const auto line_amount = lines.size( );

        constexpr auto amount_threads = 12;

        const auto add_run_thread = [ & ]( int offset ) -> void {
            for ( int i = offset; i < line_amount; i += amount_threads ) {
                if ( lines.size( ) < i ) return;

                auto parsed = parse_row(
                    lines[ i ],
                    delimiter,
                    static_cast< int32_t >( headings_size ),
                    longest_items
                );

                if ( !parsed ) continue;

                // vlt bad practice
                rows[ i ] = *parsed;
            }
        };

        std::vector< std::thread > threads;
        threads.reserve( amount_threads );
        for ( int i = 0; i < amount_threads; ++i ) {
            threads.emplace_back( add_run_thread, i + 1 );
        }

        for ( auto& thread : threads ) {
            thread.join( );
        }

        return std::make_shared< Table >( headings, rows, longest_items );
    }

    auto Table::encode( char delimiter ) const -> std::string {
        auto timer = utils::Timer::start( "Table::encode" );
        std::string out;

        out += m_headings.encode( delimiter, m_longest_input_per_column );

        constexpr auto amount_threads = 8;

        static std::array< std::string, amount_threads > output{ };
        static const auto rows_size = m_rows.size( );
        static auto rows = m_rows;
        static auto longest_input_per_column = m_longest_input_per_column;

        const auto part_size = static_cast< int32_t >( rows_size / amount_threads );

        const auto encode_rows = [ delimiter, part_size ](
            const int32_t offset
        ) -> void {
            std::string s;

            const auto start = offset * part_size;
            const auto end = offset == amount_threads - 1 ? static_cast< int32_t >( rows_size ) : start + part_size;

            for ( int i = start; i < end; ++i ) {
                if ( rows_size < i ) return;
                s += rows[ i ].encode( delimiter, longest_input_per_column ) + "\n";
            }

            // vlt bad practice
            output[ offset ] = std::move( s );
        };


        std::vector< std::thread > threads;
        threads.reserve( amount_threads );
        for ( int i = 0; i < amount_threads; ++i ) {
            threads.emplace_back( encode_rows, i );
        }

        for ( auto& thread : threads ) {
            thread.join( );
        }

        for ( int i = 0; i < amount_threads; ++i ) {
            out += output[ i ];
        }

        return out;
    }

    auto Table::parse_row(
        const std::string& line,
        const char delimiter,
        const int32_t headings_size,
        std::vector< int32_t >& longest_items
    ) -> std::optional< TableRow > {
        auto columns = utils::string::split(
            line,
            delimiter,
            headings_size
        );

        if ( columns.empty( ) ) return std::nullopt;

        for ( int j = 0; j < columns.size( ); ++j ) {
            columns[ j ] = utils::string::trim( columns[ j ] );

            if ( longest_items[ j ] < columns[ j ].size( ) )
                longest_items[ j ] = static_cast< int32_t >( columns[ j ].size( ) );
        }

        while ( columns.size( ) < headings_size ) columns.emplace_back( "" );

        return TableRow( columns );
    }
}
