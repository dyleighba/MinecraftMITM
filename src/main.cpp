#include <iostream>
#include "network/ConnectionManager.h"
#include "type/VarInt.h"
#include "network/Packet.h"
#include "type/VarLong.h"
#include "type/Position.h"
#include "type/PositionAndRotation.h"
#include "network/PacketHandler.h"
#include "network/Proxy.h"
#include <chrono>
#include <bit>

#define PRINT_PACKETS true

int64_t getTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
#pragma pack(push, 1)
struct PlayerAbilitiesPacket {
    unsigned char flags;
    float flyingSpeed;
    float fovModifier;
};
#pragma pack(pop)

// Should sniff for respawn packet too to fix bug
bool addFlyAttr(Packet & _, Proxy * proxy) {
    Packet packet = {0x31};
    packet.data = {};
    PlayerAbilitiesPacket pap {
        0x04,
        swapEndian(0.05f),
        swapEndian(0.1f)
    };
    std::span<unsigned char> papSpan {reinterpret_cast<unsigned char *>(&pap), sizeof(PlayerAbilitiesPacket)};
    packet.data.insert(packet.data.begin(), papSpan.begin(), papSpan.end());
    proxy->sendClientboundPacket(packet);
    return true;
}

int main() {
    bool flyFixEnabled = false;
    int64_t lastFlyFixPacketTime = getTime();
    ConnectionManager connectionManager = ConnectionManager();
    printf("Waiting for client connection..\n");
    Connection clientConn = connectionManager.waitForClientConnection(NetworkAddress{"127.0.0.1", 25565});
    PacketHandler clientPH(&clientConn);
    if (!clientConn.hasValidSocket()) {
        WSACleanup();
        printf("clientConn has an invalid socket\n");
        return 1;
    }
    printf("Connecting to server..\n");
    Connection serverConn = connectionManager.connectToServer(NetworkAddress{"127.0.0.1", 25566});
    if (!serverConn.hasValidSocket()) {
        WSACleanup();
        printf("serverConn has an invalid socket\n");
        return 1;
    }

    Proxy proxy(&serverConn, &clientConn);
    Position lastPlayerPos{};
    printf("Registering callbacks.\n");
    proxy.registerClientboundListener(addFlyAttr, 0x31);
    printf("Starting proxy loop.\n");
    do {
        proxy.processNextServerboundPacket();
        proxy.processNextClientboundPacket();
    } while (true);
}



