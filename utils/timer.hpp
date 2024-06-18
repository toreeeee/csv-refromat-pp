#pragma once

#include <chrono>
#include <iostream>
#include <source_location>
#include <utility>

namespace utils {
    class Timer {
    public:
        ~Timer( ) {
            print( );
        }

        static auto start( const std::string& name = std::source_location::current( ).function_name( ) ) -> Timer {
            return Timer( name );
        }

        // Timer( ): Timer( std::source_location::current( ).function_name( ) ) {
        // }

        explicit Timer( std::string name = std::source_location::current( ).function_name( ) ) : m_name(
            std::move( name ) ) {
        }

        auto print( ) -> void {
            std::cout << m_name << " took: "
                    << static_cast< float >( std::chrono::duration_cast< std::chrono::microseconds >(
                        std::chrono::high_resolution_clock::now( ) - m_start ).count( ) ) / 1000.f << "ms\n";
        }

    private:
        std::chrono::high_resolution_clock::time_point m_start = std::chrono::high_resolution_clock::now( );
        std::string m_name{ };
    };
}
