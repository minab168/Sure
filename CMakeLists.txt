cmake_minimum_required(VERSION 3.16)
project(Sure
    VERSION 0.1.0
    DESCRIPTION "A lightweight, reliable, and predictable C and C++ framework for developing bare-metal and embedded systems, designed to provide a solid foundation with zero-cost abstractions."
    LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(SURE_BUILD_TESTS "Build Sure tests and benchmarks" OFF)
option(NO_RUNTIME "Disable Runtime and VTable" ON)

if (NO_RUNTIME)
    add_compile_definitions(SURE__NO_RUNTIME_)
endif ()

if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING
        "Build configuration" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
        Debug Release RelWithDebInfo MinSizeRel)
endif ()


add_library(Sure INTERFACE)
add_library(Sure::Sure ALIAS Sure)


if(NOT CMAKE_GENERATOR STREQUAL "Ninja")
    message(STATUS "Consider using `Ninja` for faster build")
endif()


#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build/lib)
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build/lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build/bin)


add_subdirectory(Hardware)
add_subdirectory(Common)

if(SURE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(Contribs/googletest)
endif()

target_include_directories(Sure INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/>
    $<INSTALL_INTERFACE:include>
)
