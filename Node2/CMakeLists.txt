cmake_minimum_required(VERSION 3.26)
project(Node2)

set(CMAKE_CXX_STANDARD 20)

add_executable(Node2 main.cpp
        ../Shared/ilogger.h
        ../Shared/logger.cpp
        ../Shared/logger.h
        ../Shared/iacceptoradapter.h
        ../Shared/iconnection.h
        ../Shared/connection.cpp
        ../Shared/connection.h
        ../Shared/acceptoradapter.cpp
        ../Shared/acceptoradapter.h
        ../Shared/iconnectionmanager.h
        ../Shared/connectionmanager.h
        ../Shared/connectionmanager.cpp
        ../Shared/concord.cpp
        ../Shared/concord.h
        ../Shared/istrategy.h
        ../Shared/paxos.cpp
        ../Shared/paxos.h
        ../Shared/packet.h
        ../Shared/utility.h
        ../Shared/iheartbeat.h
        ../Shared/heartbeat.cpp
        ../Shared/heartbeat.h
)

find_package(Boost 1.74.0 REQUIRED)
if (Boost_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
endif()