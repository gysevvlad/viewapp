cmake_minimum_required(VERSION 3.0)

aux_source_directory(src SOURCES)
add_library(viewapp STATIC ${SOURCES})
target_include_directories(viewapp PRIVATE headers/viewapp INTERFACE headers/)
target_link_libraries(viewapp spdlog)

add_executable(reactor_test tests/reactor_test.cpp src/Reactor.cpp src/helper.cpp)
target_include_directories(reactor_test PRIVATE .)
target_include_directories(reactor_test PRIVATE src)
target_link_libraries(reactor_test gtest_main)
add_test(Reactor reactor_test)

