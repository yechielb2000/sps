cmake_minimum_required(VERSION 3.31)
project(sps)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(
        cxxopts
        GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
        GIT_TAG v3.2.0
)


FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.13.0
)

FetchContent_MakeAvailable(cxxopts spdlog)

add_executable(sps
        sps.cpp
        OptionsParser.cpp
        scanner.cpp
        logger.cpp
        logger.h
)

target_link_libraries(sps
        PRIVATE
        cxxopts
        spdlog::spdlog
)

if (WIN32)
    target_link_libraries(sps PRIVATE ws2_32)
endif ()
