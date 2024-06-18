#pragma once

#include <utility>
#include <vector>
#include "TableRow.hpp"
#include "../utils/timer.hpp"

namespace csv {
    class Table {
    public:
        Table( ) = default;

        Table(
            const std::vector< std::string >& headings,
            std::vector< TableRow > rows,
            std::vector< int32_t > longest_items
        )
            : m_headings( headings ),
              m_longest_input_per_column( std::move( longest_items ) ),
              m_rows( std::move( rows ) ) {
        }

        static auto parse( const std::string& content, char delimiter ) -> std::shared_ptr< Table >;

        [[nodiscard]] auto encode( char delimiter ) const -> std::string;

    private:
        static auto parse_row(
            const std::string& line,
            char delimiter,
            int32_t headings_size,
            std::vector< int32_t >& longest_items
        ) -> std::optional< TableRow >;

    private:
        TableRow m_headings{ };
        std::vector< int32_t > m_longest_input_per_column{ };
        std::vector< TableRow > m_rows{ };
    };
}
