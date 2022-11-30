#include <iostream>
#include "network/ConnectionManager.h"
#include "type/VarInt.h"
#include "network/Packet.h"
#include "type/VarLong.h"
#include "type/Position.h"
#include "type/PositionAndRotation.h"
#include <chrono>

#define PRINT_PACKETS true

int64_t getTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

int main() {
    bool flyFixEnabled = false;
    int64_t lastFlyFixPacketTime = getTime();
    ConnectionManager connectionManager = ConnectionManager();
    printf("Waiting for client connection..\n");
    Connection clientConn = connectionManager.waitForClientConnection("127.0.0.1", "25565");
    if (!clientConn.hasValidSocket()) {
        WSACleanup();
        printf("clientConn has an invalid socket\n");
        return 1;
    }
    printf("Connecting to server..\n");
    Connection serverConn = connectionManager.connectToServer("127.0.0.1", "25566");
    if (!serverConn.hasValidSocket()) {
        WSACleanup();
        printf("serverConn has an invalid socket\n");
        return 1;
    }
    Position lastPlayerPos{};
    printf("Starting proxy loop.\n");
    do {
        Packet clientToServer = clientConn.receivePacket();
        if (clientToServer.id != -1) {
            bool sendPacket = true;
            Position pos{};
            PositionAndRotation posAndRot{};
            switch (clientToServer.id) {
                case 0x1C: // Player abilities
                    printf("[c2s] Player Abilities (Flying: %s) DROPPED\n", (clientToServer.data[0] == 0x2) ? "true" : "false");
                    sendPacket = false;
                    flyFixEnabled = clientToServer.data[0] == 0x2;
                    break;
                case 0x14: // Set Player Position
                    //printf("[c2s] Set Player Position\n");
                    pos = Position::parse(clientToServer.data);
                    lastPlayerPos = {pos.x, pos.y, pos.z};
                    break;
                case 0x15: // Set Player Position and Rotation
                    //printf("[c2s] Set Player Position and Rotation\n");
                    posAndRot = PositionAndRotation::parse(clientToServer.data);
                    lastPlayerPos = {posAndRot.x, posAndRot.y, posAndRot.z};
                    break;
            }
            //printf("Position: x: %.1f, y: %.1f, z: %.1f\n", lastPlayerPos.x, lastPlayerPos.y, lastPlayerPos.z);
            if (sendPacket) {
                serverConn.sendPacket(clientToServer);
            }
            //printf("[c2s] [%zu]\n", clientToServer.data.size());
        }
        Packet serverToClient = serverConn.receivePacket();
        if (serverToClient.id != -1) {
            bool sendPacket = true;
            switch (serverToClient.id) {
                case 0x25:
                    printf("[s2c] Login (play)\n");
                    break;
                case 0x31:
                    printf("[s2c] Player Abilities\n");
                    sendPacket = false;
                    Packet packet{0x31};
                    packet.data.push_back(0x04);
                    float flyingSpeed = 0.1;
                    auto flyingSpeedBytes = (unsigned char*)&flyingSpeed;
                    packet.data.push_back(flyingSpeedBytes[3]);
                    packet.data.push_back(flyingSpeedBytes[2]);
                    packet.data.push_back(flyingSpeedBytes[1]);
                    packet.data.push_back(flyingSpeedBytes[0]);
                    packet.data.push_back(flyingSpeedBytes[3]);
                    packet.data.push_back(flyingSpeedBytes[2]);
                    packet.data.push_back(flyingSpeedBytes[1]);
                    packet.data.push_back(flyingSpeedBytes[0]);
                    clientConn.sendPacket(packet);
                    break;
            }
            if (sendPacket) {
                clientConn.sendPacket(serverToClient);
            }
            int64_t currentTime = getTime();
            if (flyFixEnabled && currentTime - lastFlyFixPacketTime > 2000 && !lastPlayerPos.onGround) {
                lastFlyFixPacketTime = currentTime;
                Position fakePos{lastPlayerPos.x, lastPlayerPos.y-0.1, lastPlayerPos.z, lastPlayerPos.onGround};
                Packet p = {
                        0x14,
                        fakePos.bytes()
                };
                serverConn.sendPacket(p);
                printf("Send fly fix packet %lld\n", currentTime - lastFlyFixPacketTime);
            }
            //printf("[s2c] [%zu]\n", serverToClient.data.size());
        }
    } while (true);
}



