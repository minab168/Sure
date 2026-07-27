cmake_minimum_required(VERSION 3.16)
project(Sure LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_library(Sure INTERFACE)


if(NOT CMAKE_GENERATOR STREQUAL "Ninja")
    message(STATUS "Consider using `Ninja` for faster build")
endif()


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Build/bin)


enable_testing()


add_subdirectory(Common)
#add_subdirectory(Contribs/googletest)


target_include_directories(Sure INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<INSTALL_INTERFACE:include>
)