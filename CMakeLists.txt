cmake_minimum_required(VERSION 3.26)
project(AttackOnPaxos)

set(CMAKE_CXX_STANDARD 20)

add_executable(AttackOnPaxos main.cpp
        src/iConnection.h
        src/plainconnection.h
        src/asioconnection.h)


#add_subdirectory(Node)