cmake_minimum_required(VERSION 3.0)

add_executable(app main.cpp)
add_dependencies(app viewapp)
target_link_libraries(app viewapp)
