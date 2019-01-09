cmake_minimum_required(VERSION 3.0)
project(viewapp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

aux_source_directory(src SOURCES)
add_library(viewapp STATIC ${SOURCES})
target_include_directories(viewapp PRIVATE headers/viewapp INTERFACE headers/)
