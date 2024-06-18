cmake_minimum_required(VERSION 3.29)
project(csv_formatter)

set(CMAKE_CXX_STANDARD 26)

add_executable(csv_formatter main.cpp
        csv/Table.hpp
        csv/TableRow.hpp
        utils/fs.hpp
        utils/timer.hpp
        csv/Table.cpp
        utils/string.hpp
        csv/TableRow.cpp
        utils/io.hpp)
