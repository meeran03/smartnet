cmake_minimum_required(VERSION 3.24)
project(untitled)

set(CMAKE_CXX_STANDARD 14)

INCLUDE_DIRECTORIES(SYSTEM "/usr/include/graphviz/")

add_executable(untitled nn.cpp)

target_link_libraries(untitled PUBLIC cgraph gvc)
