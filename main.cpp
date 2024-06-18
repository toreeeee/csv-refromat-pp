#include <iostream>
#include <fstream>
#include "csv/Table.hpp"
#include "utils/fs.hpp"
#include "utils/io.hpp"

int main( const int argc, char* argv[ ] ) {
    utils::Timer timer( "reformatting" );
    const auto input_file = argv[ argc - 1 ];

    if ( !std::filesystem::exists( input_file ) ) {
        utils::io::println( "Input file not found" );
        return 1;
    }

    const auto str = utils::fs::read_all( input_file );

    const auto table = csv::Table::parse( *str, ':' );
    const auto encoded = table->encode( ':' );

    if ( !utils::fs::write_all( input_file, encoded ).has_value( ) ) {
        utils::io::println( "Could not write output to file" );
        return 1;
    }

    return 0;
}
