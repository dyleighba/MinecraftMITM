#include <iostream>
#include "network/ConnectionManager.h"
#include "type/VarInt.h"
#include "network/Packet.h"
#include "type/VarLong.h"
#include "type/Position.h"
#include "type/PositionAndRotation.h"

#define PRINT_PACKETS true

Position parsePositionData(std::vector<unsigned char> positionData) {
    std::reverse(positionData.begin(), positionData.end());
    Position pos {
            *(double*)(positionData.data()+17),
            *(double*)(positionData.data()+9),
            *(double*)(positionData.data()+1),
            *(bool*)positionData.data()
    };
    return pos;
}

PositionAndRotation parsePositionAndRotationData(std::vector<unsigned char> positionData) {
    std::reverse(positionData.begin(), positionData.end());
    PositionAndRotation pos {
            *(double*)(positionData.data()+25),
            *(double*)(positionData.data()+17),
            *(double*)(positionData.data()+9),
            *(float*)(positionData.data()+5),
            *(float*)(positionData.data()+1),
             *(bool*)(positionData.data())
    };
    return pos;
}

Packet generatePositionPacket(Position pos) {
    Packet newPacket{0x14, {}};
    double altY = pos.y-0.01;
    unsigned char* pointers[3] {
        (unsigned char*)&pos.x,
        (unsigned char*)&altY,
        (unsigned char*)&pos.z
    };
    unsigned char rawPacketData[(sizeof(double)*3)+1]{};
    for (int n = 0; n < 3; ++n) {
        for (int i = 0; i < 8; ++i) {
            rawPacketData[(n*8)+i] = pointers[n][i];
            //newPacket.data.push_back(pointers[n][i]);
        }
    }
    rawPacketData[(sizeof(double)*3)] = 0x00;
    //newPacket.data.push_back(0x00);
    std::vector<unsigned char> bytes(rawPacketData, (rawPacketData)+(sizeof(double)*3)+1);
    newPacket.data = bytes;
    return newPacket;
}

int main() {
    bool flyFixEnabled = false;
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
            Position pos;
            PositionAndRotation posAndRot;
            switch (clientToServer.id) {
                case 0x1C: // Player abilities
                    printf("[c2s] Player Abilities (Flying: %s) DROPPED\n", (clientToServer.data[0] == 0x2) ? "true" : "false");
                    sendPacket = false;
                    break;
                case 0x14: // Set Player Position
                    //printf("[c2s] Set Player Position\n");
                    pos = parsePositionData(clientToServer.data);
                    lastPlayerPos = {pos.x, pos.y, pos.z};
                    break;
                case 0x15: // Set Player Position and Rotation
                    //printf("[c2s] Set Player Position and Rotation\n");
                    posAndRot = parsePositionAndRotationData(clientToServer.data);
                    lastPlayerPos = {posAndRot.x, posAndRot.y, posAndRot.z};
                    break;
            }
            printf("Position: x: %.1f, y: %.1f, z: %.1f\n", lastPlayerPos.x, lastPlayerPos.y, lastPlayerPos.z);
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
                    flyFixEnabled = true;
                    break;
                case 0x31:
                    printf("[s2c] Player Abilities\n");
                    sendPacket = false;
                    Packet packet{0x31};
                    packet.data.push_back(0x04);
                    float flyingSpeed = 0.1;
                    unsigned char *flyingSpeedBytes = (unsigned char*)&flyingSpeed;
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
            if (flyFixEnabled && !lastPlayerPos.onGround) {
                Packet p = generatePositionPacket(lastPlayerPos);
                serverConn.sendPacket(p);
            }
            //printf("[s2c] [%zu]\n", serverToClient.data.size());
        }
    } while (true);
}



