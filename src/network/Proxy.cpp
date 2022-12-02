//
// Created by Dylan on 12/2/2022.
//

#include "Proxy.h"

#include <utility>

Proxy::Proxy(Connection *server, Connection *client)
        : server(PacketHandler(server)), client(PacketHandler(client)),
        serverConn(server), clientConn(client) {}

bool Proxy::registerClientboundListener(bool (*callable)(Packet, Proxy *), int packetId) {
    return registerListener(callable, packetId, clientboundCallbacks);
}

bool Proxy::removeClientboundListener(bool (*callable)(Packet, Proxy *), int packetId) {
    return removeListener(callable, packetId, clientboundCallbacks);
}

bool Proxy::registerServerboundListener(bool (*callable)(Packet, Proxy *), int packetId) {
    return registerListener(callable, packetId, serverboundCallbacks);
}

bool Proxy::removeServerboundListener(bool (*callable)(Packet, Proxy *), int packetId) {
    return removeListener(callable, packetId, serverboundCallbacks);
}

bool Proxy::registerListener(bool (*callable)(Packet, Proxy *), int packetId,
                             std::map<int, std::set<bool (*)(Packet, Proxy *)>> & callbackArr) {
    if (packetId < -1) return false;
    if (!callbackArr.contains(packetId)) {
        callbackArr[packetId] = std::set<bool(*)(Packet, Proxy *)>{};
    }
    if (callbackArr[packetId].contains(callable)) return false;
    callbackArr[packetId].insert(callable);
    return true;
}

bool Proxy::removeListener(bool (*callable)(Packet, Proxy *), int packetId,
                           std::map<int, std::set<bool (*)(Packet, Proxy *)>> & callbackArr) {
    if (!callbackArr.contains(packetId)) return false;
    if (!callbackArr[packetId].contains(callable)) return false;
    callbackArr[packetId].erase(callable);
    if (callbackArr[packetId].empty()) {
        callbackArr.erase(packetId);
    }
    return true;
}

bool Proxy::runCallbacks(const Packet& packet, std::map<int, std::set<bool(*)(Packet, Proxy *)>> & callbackArr) {
    bool dropPacket = false;
    if (packet.id < 0) return dropPacket;
    if (packet.data.empty()) return dropPacket;
    int callbackPacketId = packet.id;
    if (!callbackArr.contains(packet.id)) {
        if (!callbackArr.contains(-1)) return dropPacket;
        callbackPacketId = -1;
    };
    if (callbackArr[callbackPacketId].empty()) return dropPacket;
    for (bool(*func)(Packet, Proxy *) : callbackArr[callbackPacketId]) {
        dropPacket = dropPacket || func(packet, this);
    }
    return dropPacket;
}

void Proxy::sendClientboundPacket(Packet packet) {
    client.sendPacket(std::move(packet));
}

void Proxy::sendServerboundPacket(Packet packet) {
    server.sendPacket(std::move(packet));
}

void Proxy::processNextClientboundPacket() {
    Packet packet = server.receivePacket();
    if (packet.id <= 0) return;
    if (packet.data.empty()) return;
    if (runCallbacks(packet, clientboundCallbacks)) return;
    client.sendPacket(packet);
}

void Proxy::processNextServerboundPacket() {
    Packet packet = client.receivePacket();
    if (packet.id <= 0) return;
    if (packet.data.empty()) return;
    if (runCallbacks(packet, serverboundCallbacks)) return;
    server.sendPacket(packet);
}

