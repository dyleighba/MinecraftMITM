cmake_minimum_required(VERSION 3.23)
project(MinecraftMITM)

set(CMAKE_CXX_STANDARD 23)

link_libraries(ws2_32 wsock32)

include_directories(src/)
add_executable(MinecraftMITM
                src/main.cpp src/type/VarNumber.cpp src/network/Connection.cpp
                src/network/ConnectionManager.cpp src/type/VarInt.cpp src/type/VarLong.cpp
                src/type/PositionAndRotation.cpp src/type/Position.cpp src/network/ByteManipulation.cpp)


Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests src/test.cpp src/type/VarNumber.cpp src/network/Connection.cpp
        src/network/ConnectionManager.cpp src/type/VarInt.cpp src/type/VarLong.cpp
        src/type/PositionAndRotation.cpp src/type/Position.cpp src/network/ByteManipulation.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)