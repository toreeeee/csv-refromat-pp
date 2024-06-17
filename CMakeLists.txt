cmake_minimum_required(VERSION 3.29)
project(csv_formatter)

set(CMAKE_CXX_STANDARD 26)

add_executable(csv_formatter main.cpp
        csv/Table.hpp
        csv/TableRow.hpp
        csv/parse.hpp
        utils/fs.hpp
        utils/timer.hpp)
