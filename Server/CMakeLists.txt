cmake_minimum_required(VERSION 3.26)
project(AttackOnPaxos)

set(CMAKE_CXX_STANDARD 20)

add_executable(AttackOnPaxos main.cpp
        ../Shared/iConnection.h
        ../Shared/asioconnection.h
        ../Shared/ilogger.h
        ../Shared/logger.cpp
        ../Shared/logger.h
        ../Shared/iacceptoradapter.h
        ../Shared/isocketadapter.h
        ../Shared/socketadapter.cpp
        ../Shared/socketadapter.h
        ../Shared/acceptoradapter.cpp
        ../Shared/acceptoradapter.h
        ../Shared/iconnectionmanager.h
        ../Shared/connectionmanager.h
        ../Shared/connectionmanager.cpp
)

find_package(Boost 1.74.0 REQUIRED)
if (Boost_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
endif()
#add_subdirectory(Node)