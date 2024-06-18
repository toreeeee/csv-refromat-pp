#pragma once
#include <__format/format_functions.h>

namespace utils::io {
    template< class... Args >
    auto print( const std::string_view _fmt, Args&&... _args ) -> void {
        std::cout << std::vformat( _fmt, std::make_format_args( _args... ) );
    }

    template< class... Args >
    auto println( const std::string_view _fmt, Args&&... _args ) -> void {
        std::cout << std::vformat( _fmt, std::make_format_args( _args... ) ) << "\n";
    }
}
