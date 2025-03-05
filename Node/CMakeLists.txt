cmake_minimum_required(VERSION 3.26)
project(Node)

set(CMAKE_CXX_STANDARD 20)

add_executable(Node main.cpp
        ../Shared/iConnection.h
        ../Shared/plainconnection.h)

find_package(Boost 1.74.0 REQUIRED)
if (Boost_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
endif()