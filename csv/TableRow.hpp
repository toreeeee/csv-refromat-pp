#pragma once

#include <string>
#include <utility>
#include <vector>
#include <format>
#include <sstream>

#include "../utils/timer.hpp"

namespace csv {
    class TableRow {
    public:
        TableRow( ) {
        }

        TableRow( std::string line ) {
            //´
        }

        explicit TableRow( const std::vector< std::string >& columns ) : m_columns( columns ) {
        }


        [[nodiscard]] auto encode(
            char delimiter,
            const std::vector< int32_t >& column_sizes
        ) const -> std::string;

    private:
        static auto get_spaces( int32_t amount ) -> std::string;

    private:
        std::vector< std::string > m_columns{ };
    };
}
