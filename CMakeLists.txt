cmake_minimum_required(VERSION 3.21.1)
project(TreeHSE)

set(CMAKE_CXX_STANDARD 17)

add_executable(TreeHSE main.cpp)

enable_testing()
add_subdirectory(doctest)

add_executable(TreeHSE_test test/test.cpp)
target_link_libraries(TreeHSE_test doctest)