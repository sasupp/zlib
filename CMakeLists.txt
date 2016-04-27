cmake_minimum_required(VERSION 2.8)

# solution name
project(sasupp)

set(CMAKE_FILES_DIRECTORY ${CMAKE_SOURCE_DIR}/../cmake)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/../bin)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Set compiler flags and options. 
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall")

include_directories("${CMAKE_SOURCE_DIR}/zlib")

add_executable(zlib-tests zlib-tests/main.cpp)
