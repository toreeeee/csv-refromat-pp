#pragma once

#include <chrono>
#include <iostream>
#include <utility>

namespace utils {
    class Timer {
    public:
        ~Timer( ) {
            print( );
        }

        Timer( ) { }

        explicit Timer( std::string name ) : m_name( std::move( name ) ) { }

        auto print( ) -> void {
            std::cout << m_name << " took: "
                      << static_cast<float>( std::chrono::duration_cast< std::chrono::microseconds >(
                              std::chrono::high_resolution_clock::now( ) - m_start ).count( )) / 1000.f << "ms\n";
        }

    private:
        std::chrono::high_resolution_clock::time_point m_start = std::chrono::high_resolution_clock::now( );
        std::string m_name{ };
    };
}