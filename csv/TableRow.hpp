#pragma once

#include <string>
#include <utility>
#include <vector>
#include <format>
#include <sstream>

namespace csv {
    class TableRow {
    public:
        TableRow() {
        }

        TableRow(std::string line) {
            //´
        }

        explicit TableRow(const std::vector< std::string >& columns) : m_columns(columns) {
        }


        [[nodiscard]] auto encode(const char delimiter,
                                  const std::vector< int32_t >& column_sizes) const -> std::string {
            std::string ss;
            for (int i = 0; i < m_columns.size(); ++i) {
                const auto col_length = column_sizes[ i ] + 6;
                const auto current_col_length = static_cast< int32_t >( m_columns[ i ].size() );

                const auto spaces_front = static_cast< float >( col_length - current_col_length ) / 2.f;
                const auto spaces_end =
                        static_cast< float >( col_length ) - (
                            spaces_front + static_cast< float >( current_col_length ));

                if (i != 0) { ss += delimiter; }

                ss += get_spaces(( int ) spaces_front) + m_columns[ i ] +
                        get_spaces(( int ) std::ceil(spaces_end));
            }

            return ss;
        }

    private:
        static auto get_spaces(const int32_t amount) -> std::string {
            std::string ss;
            for (int i = 0; i < amount; ++i) {
                ss += " ";
            }
            return ss;
        }

    private:
        std::vector< std::string > m_columns{ };
    };
}
