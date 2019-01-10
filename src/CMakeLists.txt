cmake_minimum_required(VERSION 3.0)

aux_source_directory(src SOURCES)
add_library(viewapp STATIC ${SOURCES})
target_include_directories(viewapp PRIVATE headers/viewapp INTERFACE headers/)
