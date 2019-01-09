cmake_minimum_required(VERSION 3.0)
project(app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(app main.cpp)
add_subdirectory(viewapp)
target_link_libraries(app viewapp)