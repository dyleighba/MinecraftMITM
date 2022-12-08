//
// Created by Dylan on 12/1/2022.
//

#include "PacketHandler.h"

PacketHandler::PacketHandler(Connection * connection) {
    this->connection = connection;
}

bool PacketHandler::registerPacketListener(void(*callable)(Packet), int packetId) {
    if (packetId < -1) return false;
    if (!callbacks.contains(packetId)) {
        callbacks[packetId] = std::set<void(*)(Packet)>{};
    }
    if (callbacks[packetId].contains(callable)) return false;
    callbacks[packetId].insert(callable);
    return true;
}

bool PacketHandler::removePacketListener(void(*callable)(Packet), int packetId) {
    if (!callbacks.contains(packetId)) return false;
    if (!callbacks[packetId].contains(callable)) return false;
    callbacks[packetId].erase(callable);
    if (callbacks[packetId].empty()) {
        callbacks.erase(packetId);
    }
    return true;
}

void PacketHandler::processNextIncomingPacket() {
    Packet packet = receivePacket();
    if (packet.data.empty()) return;
    if (packet.id <= 0) return;
    alertPacketListeners(packet);
}

void PacketHandler::sendPacket(Packet packet) {
    std::vector<unsigned char> byteArr{};
    byteArr.reserve(packet.data.size()+10);

    std::vector<unsigned char> packetId = VarInt{packet.id}.bytes();
    byteArr.insert(byteArr.end(), packetId.begin(), packetId.end());
    byteArr.insert(byteArr.end(), packet.data.begin(), packet.data.end());

    std::vector<unsigned char> packetSize = VarInt{(static_cast<int>(byteArr.size()))}.bytes();
    byteArr.insert(byteArr.begin(), packetSize.begin(), packetSize.end());
    connection->sendRawData(byteArr);
}

Packet PacketHandler::unpackPacket(const std::vector<unsigned char> & packetBytes) {
    Packet packet{-1};
    if (packetBytes.empty()) { return packet; }
    int length = VarInt::fromBytes(packetBytes);
    int varIntLength = static_cast<int>(VarInt{length}.bytes().size());
    std::vector<unsigned char> packetIdVec{packetBytes.begin()+varIntLength, packetBytes.end()}; // Lots of copy but no error on small packet
    packet.id = VarInt::fromBytes(packetIdVec);
    int packetIdLength = static_cast<int>(VarInt{packet.id}.bytes().size());
    packet.data = std::vector<unsigned char>{packetBytes.begin()+varIntLength+packetIdLength, packetBytes.end()};
    if (length-packetIdLength != (packet.data.size())) {
        printf("Something went wrong! Invalid packet size (len: %i, size: %zu)\n", length-packetIdLength, packet.data.size());
        // A test should be made to make sure its not a bug in code but rather input data
        // Or perhaps it should throw
    }
    return packet;
}

void PacketHandler::alertPacketListeners(Packet packet) {
    if (packet.id < 0) return;
    if (packet.data.empty()) return;
    int callbackPacketId = packet.id;
    if (!callbacks.contains(packet.id)) {
        if (!callbacks.contains(-1)) return;
        callbackPacketId = -1;
    };
    if (callbacks[callbackPacketId].empty()) return;
    for (void(*func)(Packet) : callbacks[callbackPacketId]) {
        func(packet);
    }
}

Packet PacketHandler::receivePacket() {
    Packet packet = {-1};
    // this is from Connection::receivePacket()
    int length = connection->getNextPacketLength();
    if (length <= 0) return packet;
    int varIntLength = static_cast<int>(VarInt{length}.bytes().size());
    std::vector<unsigned char> rawPacket = connection->receiveRawData(length+varIntLength, false);
    return unpackPacket(rawPacket);
}
