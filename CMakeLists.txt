# Minimum CMake version and project definition
cmake_minimum_required(VERSION 3.20)
project(sps
    VERSION 1.0.0
    DESCRIPTION "Simple Port Scanner"
    LANGUAGES CXX
)

# Global configuration
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif()

# Compiler warnings
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        $<$<BOOL:${WIN32}>:-Wno-unknown-pragmas>  # Disable unknown pragmas warn ing on MinGW
    )
endif()

# External dependencies
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

# Main executable
add_executable(${PROJECT_NAME}
    sps.cpp
    option_parser.cpp
    option_parser.hpp
    scanner.cpp
    scanner.hpp
    logger.cpp
    logger.hpp
)

# Target properties and linking
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        cxxopts
        spdlog::spdlog
)

# Platform-specific configuration
if(WIN32)
    target_link_libraries(${PROJECT_NAME} 
        PRIVATE 
            ws2_32
    )
endif()

# Installation rules
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Export compile commands for tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable testing if needed
option(BUILD_TESTING "Build tests" OFF)
if(BUILD_TESTING)
    enable_testing()
endif()

message(STATUS "")
message(STATUS "Project configuration:")
message(STATUS "  Version:        ${PROJECT_VERSION}")
message(STATUS "  Build type:     ${CMAKE_BUILD_TYPE}")
message(STATUS "  Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "  Compiler:       ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "")