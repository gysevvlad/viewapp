cmake_minimum_required(VERSION 3.0)

project(viewapp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_subdirectory(src)
add_subdirectory(examples)
add_subdirectory(googletest)

