#pragma once

#include <string>
#include <fstream>
#include <sstream>

#include <expected>

namespace utils::fs {
    enum class FsError : int32_t {
        CouldNotReadFile,
        CouldNotWriteFile
    };

    auto write_all( std::string_view path, std::string_view content ) -> std::expected< void, FsError > {
        try {
            std::ofstream stream( path );
            stream << content;
            return { };
        } catch ( ... ) {
            return std::unexpected( FsError::CouldNotWriteFile );
        }
    }

    auto read_all( std::string_view path ) -> std::expected< std::string, FsError > {
        try {
            std::stringstream ss;
            std::ifstream stream( path );
            ss << stream.rdbuf( );

            return ss.str( );
        } catch ( ... ) {
            return std::unexpected( FsError::CouldNotReadFile );
        }
    }
}