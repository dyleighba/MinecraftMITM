//
// Created by Dylan on 12/1/2022.
//

#ifndef MINECRAFTMITM_PACKETHANDLER_H
#define MINECRAFTMITM_PACKETHANDLER_H


#include <vector>
#include <map>
#include <network/Connection.h>
#include <network/Packet.h>
#include <array>
#include <set>

class PacketHandler {
private:
    Connection * connection;
    std::map<int, std::set<void(*)(Packet)>> callbacks;
    void alertPacketListeners(Packet packet);
public:
    explicit PacketHandler(Connection * connection);
    bool registerPacketListener(void(*callable)(Packet), int packetId);
    bool removePacketListener(void(*callable)(Packet), int packetId);
    void processNextIncomingPacket();
    void sendPacket(Packet packet);
    Packet receivePacket();
    static Packet unpackPacket(const std::vector<unsigned char> & packetBytes);

};


#endif //MINECRAFTMITM_PACKETHANDLER_H
