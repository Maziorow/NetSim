cmake_minimum_required(VERSION 3.13)
project(implementation)

set(CMAKE_CXX_STANDARD 17)
set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++17 -Wall -Wextra -Werror -Wpedantic -pedantic-errors")

include_directories(
        include
)

add_definitions(${GCC_COVERAGE_COMPILE_FLAG})

set(SOURCE_FILES
        src/package.cpp
        src/storage_types.cpp
        )
