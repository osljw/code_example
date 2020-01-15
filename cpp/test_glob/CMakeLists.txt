project(glob CXX)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -std=c++11")
add_library(glob STATIC glob.cpp glob.h)

add_executable(glob_test glob_test.cpp)
target_link_libraries(glob_test glob)
