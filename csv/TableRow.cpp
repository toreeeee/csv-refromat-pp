#include "TableRow.hpp"

namespace csv {
    auto TableRow::encode( const char delimiter, const std::vector< int32_t >& column_sizes ) const -> std::string {
        std::string ss;
        for ( int i = 0; i < m_columns.size( ); ++i ) {
            const auto col_length = column_sizes[ i ] + 6;
            const auto current_col_length = static_cast< int32_t >( m_columns[ i ].size( ) );

            const auto spaces_front = static_cast< float >( col_length - current_col_length ) / 2.f;
            const auto spaces_end =
                    static_cast< float >( col_length ) - (
                        spaces_front + static_cast< float >( current_col_length ) );

            if ( i != 0 ) {
                ss += delimiter;
            }

            ss += get_spaces( static_cast< int >( spaces_front ) ) + m_columns[ i ] +
                    get_spaces( static_cast< int >( std::ceil( spaces_end ) ) );
        }

        return ss;
    }

    auto TableRow::get_spaces( const int32_t amount ) -> std::string {
        std::string ss;
        for ( int32_t i = 0; i < amount; ++i ) {
            ss += " ";
        }
        return ss;
    }
}
