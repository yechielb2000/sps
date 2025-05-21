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

add_executable(simple_ports_scanner
        simple-ports-scanner.cpp
        OptionsParser.cpp
        scanner.cpp
)

target_link_libraries(simple_ports_scanner PRIVATE cxxopts)

# On Windows, add ws2_32 for sockets
if (WIN32)
    target_link_libraries(simple_ports_scanner PRIVATE ws2_32)
endif()
