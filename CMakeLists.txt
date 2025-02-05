cmake_minimum_required(VERSION 3.29)
project(AttackOnPaxos)

set(CMAKE_CXX_STANDARD 20)

add_executable(AttackOnPaxos main.cpp
        src/iConnection.h
        src/plainconnection.h)


add_subdirectory(Node)