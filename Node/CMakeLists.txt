cmake_minimum_required(VERSION 3.29)
project(Node)

set(CMAKE_CXX_STANDARD 20)

add_executable(Node main.cpp
        ../src/iConnection.h)
