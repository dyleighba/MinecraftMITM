cmake_minimum_required(VERSION 3.23)
project(MinecraftMITM)

set(CMAKE_CXX_STANDARD 23)

# Download and Include Catch2
Include(FetchContent)
FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v3.0.1 # or a later release
)
FetchContent_MakeAvailable(Catch2)
# Link WinSock
link_libraries(ws2_32 wsock32)


# include source tree for every target
include_directories(src/)
add_library(MinecraftProtocol
        src/type/VarNumber.cpp src/network/Connection.cpp
        src/network/ConnectionManager.cpp src/type/VarInt.cpp src/type/VarLong.cpp
        src/type/PositionAndRotation.cpp src/type/Position.cpp src/network/ByteManipulation.cpp src/network/PacketHandler.cpp src/network/PacketHandler.h)


add_executable(MinecraftMITM src/main.cpp)
target_link_libraries(MinecraftMITM PRIVATE MinecraftProtocol)

add_executable(tests test/type/test_Position.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain MinecraftProtocol)