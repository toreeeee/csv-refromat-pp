#pragma once

#include <string>
#include <utility>
#include <vector>
#include <format>
#include <sstream>

namespace csv {
    class TableRow {
    public:
        TableRow( ) { }

        TableRow( std::string line ) {
            //´
        }

        explicit TableRow( const std::vector< std::string >& columns ) : m_columns( columns ) {
        }


        [[nodiscard]] auto encode( char delimiter, const std::vector< int32_t >& column_sizes ) const -> std::string {
            std::string ss;
            for ( int i = 0; i < m_columns.size( ); ++i ) {
                auto col_length = column_sizes[ i ] + 6;
                auto val_length = static_cast<int32_t>(m_columns[ i ].size( ));
                const auto spaces_before = static_cast<float>(col_length - val_length) / 2.f;
                const auto spaces_end =
                        static_cast<float>(col_length) - ( spaces_before + static_cast<float>(val_length) );

                if ( i != 0 )
                    ss += delimiter;

                ss += get_spaces( ( int ) spaces_before ) + m_columns[ i ] +
                      get_spaces( ( int ) std::ceil( spaces_end ) );
            }

            return ss;
        }

    private:
        static auto get_spaces( int32_t amount ) -> std::string {
            std::stringstream ss;
            for ( int i = 0; i < amount; ++i ) {
                ss << " ";
            }
            return ss.str( );
        }

    private:
        std::vector< std::string > m_columns{ };
    };
}