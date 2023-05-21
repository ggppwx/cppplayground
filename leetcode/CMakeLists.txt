cmake_minimum_required(VERSION 3.0.0)
project(playground VERSION 0.1.0)

enable_testing()


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs/googletest)

add_executable(playground playground.cpp)


target_link_libraries(playground gtest gtest_main)


include(GoogleTest)
gtest_discover_tests(playground)
