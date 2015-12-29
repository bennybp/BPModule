cmake_minimum_required(VERSION 2.8)
project(Graph CXX)

find_package(Boost REQUIRED)

#We require C++11 because TA and CTF do
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${Boost_INCLUDE_DIRS})


set(GraphHeaders ../AddEdgeWrapper.hpp
                 ../BFS.hpp
                 ../FillMacro.h
                 ../Graph.hpp
                 ../GraphItr.hpp
                 ../FindSubGraph.hpp
)

add_executable(GraphTest main.cpp)
add_dependencies(GraphTest ${GraphHeaders})

