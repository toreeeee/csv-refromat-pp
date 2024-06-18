#pragma once
#include <string>
#include <vector>

namespace utils::string {
    inline auto trim( std::string& str ) -> std::string {
        str.erase( str.find_last_not_of( ' ' ) + 1 );
        str.erase( 0, str.find_first_not_of( ' ' ) );
        return str;
    }

    inline auto split(
        const std::string& input,
        const char split_by,
        const int32_t reserve = -1
    ) -> std::vector< std::string > {
        std::string current;
        std::vector< std::string > lines;
        if ( reserve > 0 ) lines.reserve( reserve );

        for ( const auto c : input ) {
            if ( c == split_by ) {
                lines.emplace_back( current );
                current.clear( );
            } else {
                current += c;
            }
        }

        if ( !current.empty( ) ) lines.emplace_back( current );

        return lines;
    }
}
