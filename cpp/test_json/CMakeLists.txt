project(glob CXX)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -std=c++11")
# add_library(glob STATIC test_json.cpp)

add_executable(test_json test_json.cpp)
# target_link_libraries(glob_test glob)
