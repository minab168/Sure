cmake_minimum_required(VERSION 3.16)
project(Sure
    VERSION 0.1.0
    DESCRIPTION "A lightweight, reliable, and predictable C and C++ framework for developing bare-metal and embedded systems, designed to provide a solid foundation with zero-cost abstractions."
    LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Build configuration" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        Debug Release RelWithDebInfo MinSizeRel)
endif ()


add_library(Sure INTERFACE)


if(NOT CMAKE_GENERATOR STREQUAL "Ninja")
    message(STATUS "Consider using `Ninja` for faster build")
endif()


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build/bin)


add_subdirectory(Contribs/googletest)
add_subdirectory(Common)


enable_testing()


target_include_directories(Sure INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>
)