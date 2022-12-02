//
// Created by Dylan on 12/2/2022.
//

#ifndef MINECRAFTMITM_PROXY_H
#define MINECRAFTMITM_PROXY_H


#include <network/NetworkAddress.h>
#include <network/Connection.h>
#include <map>
#include <set>
#include <network/PacketHandler.h>

class Proxy {
private:
    const Connection * serverConn;
    const Connection * clientConn;
    PacketHandler server;
    PacketHandler client;
    std::map<int, std::set<bool(*)(Packet, Proxy *)>> clientboundCallbacks;
    std::map<int, std::set<bool(*)(Packet, Proxy *)>> serverboundCallbacks;
    bool runCallbacks(const Packet& packet, std::map<int, std::set<bool(*)(Packet, Proxy *)>> & callbackArr);
    static bool registerListener(bool(*callable)(Packet, Proxy *), int packetId, std::map<int, std::set<bool(*)(Packet, Proxy *)>> & callbackArr);
    static bool removeListener(bool(*callable)(Packet, Proxy *), int packetId, std::map<int, std::set<bool(*)(Packet, Proxy *)>> & callbackArr);
public:
    Proxy(Connection *server, Connection *client);
    bool registerClientboundListener(bool(*callable)(Packet, Proxy *), int packetId);
    bool removeClientboundListener(bool(*callable)(Packet, Proxy *), int packetId);
    bool registerServerboundListener(bool(*callable)(Packet, Proxy *), int packetId);
    bool removeServerboundListener(bool(*callable)(Packet, Proxy *), int packetId);
    void processNextClientboundPacket();
    void processNextServerboundPacket();
    void sendClientboundPacket(Packet packet);
    void sendServerboundPacket(Packet packet);
};


#endif //MINECRAFTMITM_PROXY_H
