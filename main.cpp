#include <iostream>
#include <fstream>
#include "csv/Table.hpp"
#include "utils/fs.hpp"

int main( int argc, char* argv[] ) {
    const auto input_file = argv[ argc - 1 ];

    if ( !std::filesystem::exists( input_file ) ) {
        std::cout << "input file not found\n";
        return 1;
    }
    auto str = utils::fs::read_all( input_file );

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now( );

    auto table = csv::Table::parse( *str, ':' );
    auto encoded = table->encode( ':' );

    auto time_passed = std::chrono::high_resolution_clock::now( ) - start;

    if ( !utils::fs::write_all( input_file, encoded ).has_value( ) ) {
        std::cout << "could not write file\n";
        return 1;
    }
    std::cout << static_cast<float>(std::chrono::duration_cast< std::chrono::microseconds >( time_passed ).count( )) /
                 1000.f << "ms\n";

    return 0;
}
