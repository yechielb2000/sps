cmake_minimum_required(VERSION 3.31)
project(simple_ports_scanner)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
        GIT_TAG v3.2.0
)
FetchContent_MakeAvailable(cxxopts)

add_executable(simple_ports_scanner simple-ports-scanner.cpp
        OptionsParser.hpp
        OptionsParser.cpp
)

target_link_libraries(simple_ports_scanner PRIVATE cxxopts)

target_include_directories(simple_ports_scanner PRIVATE ${cxxopts_SOURCE_DIR}/include)
