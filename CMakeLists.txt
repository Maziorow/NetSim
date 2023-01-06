cmake_minimum_required(VERSION 3.13)
project(NetSim)

set(CMAKE_CXX_STANDARD 17)
set(GCC_COVERAGE_COMPILE_FLAGS "-std=c++17 -Wall -Wextra -Werror -Wpedantic -pedantic-errors")
set(SOURCE_FILES
        src/package.cpp
        src/storage_types.cpp)

add_executable(NetSim main.cpp src/package.cpp include/storage_types.hpp src/storage_types.cpp include/config.hpp)

include_directories(
        include
)

add_definitions(${GCC_COVERAGE_COMPILE_FLAG})

target_compile_definitions(NetSim PUBLIC EXERCISE_ID=EXERCISE_ID_PACKAGES)

set(SOURCES_FILES_TESTS
        test/test_product.cpp
        test/test_catalogue.cpp
        )

add_executable(NetSim__test ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp)

# TODO: Poniższe ścieżki prawdopodobnie musisz zmienić...
target_include_directories(NetSim__test PUBLIC
../../googletest-master/googlemock/include
../../googletest-master/googletest/include
)

# TODO: Poniższą ścieżkę prawdopodobnie musisz zmienić...
add_subdirectory(../../googletest-master googletest-master)

target_link_libraries(catalogue_1__test gmock)

