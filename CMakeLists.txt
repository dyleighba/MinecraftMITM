cmake_minimum_required(VERSION 3.23)
project(MinecraftMITM)

set(CMAKE_CXX_STANDARD 23)

link_libraries(ws2_32 wsock32)
include_directories(src/)

add_executable(MinecraftMITM
                src/main.cpp src/type/VarNumber.cpp src/network/Connection.cpp
                src/network/ConnectionManager.cpp src/type/VarInt.cpp src/type/VarLong.cpp src/type/Position.h src/type/PositionAndRotation.h)
