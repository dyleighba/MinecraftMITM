//
// Created by Dylan on 12/1/2022.
//

#include "PacketHandler.h"

PacketHandler::PacketHandler(Connection * connection) {
    this->connection = connection;
}

bool PacketHandler::registerPacketListener(void(*callable)(Packet), int packetId) {
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
    // this is from Connection::receivePacket()
    int length = connection->getNextPacketLength();
    if (length <= 0) return;
    int varIntLength = static_cast<int>(VarInt::toBytes(length).size());
    std::vector<unsigned char> rawPacket = connection->receiveRawData(length+varIntLength, false);
    alertPacketListeners(unpackPacket(rawPacket));
}

void PacketHandler::sendPacket(Packet packet) {
    std::vector<unsigned char> byteArr{};
    byteArr.reserve(packet.data.size()+10);

    std::vector<unsigned char> packetId = VarInt::toBytes(packet.id);
    byteArr.insert(byteArr.end(), packetId.begin(), packetId.end());
    byteArr.insert(byteArr.end(), packet.data.begin(), packet.data.end());

    std::vector<unsigned char> packetSize = VarInt::toBytes(static_cast<int>(byteArr.size()));
    byteArr.insert(byteArr.begin(), packetSize.begin(), packetSize.end());
    connection->sendRawData(byteArr);
}

Packet PacketHandler::unpackPacket(std::vector<unsigned char> packetBytes) {
    Packet packet{-1};
    int length = VarInt::fromBytes(packetBytes);
    int varIntLength = static_cast<int>(VarInt::toBytes(length).size());
    if (packetBytes.empty()) { return packet; }
    packetBytes.erase(packetBytes.begin(), packetBytes.begin()+varIntLength);
    packet.id = VarInt::fromBytes(packetBytes);
    int packetIdLength = static_cast<int>(VarInt::toBytes(packet.id).size());
    packetBytes.erase(packetBytes.begin(), packetBytes.begin()+packetIdLength);
    packet.data = packetBytes;
    if (length != packet.data.size()) {
        printf("Something went wrong! Invalid packet size"); // A test should be made to make sure its not a bug in code but rather input data
        // Or perhaps it should throw
    }
    return packet;
}

void PacketHandler::alertPacketListeners(Packet packet) {
    if (packet.id < 0) return;
    if (packet.data.empty()) return;
    if (!callbacks.contains(packet.id)) return;
    if (callbacks[packet.id].empty()) return;
    for (void(*cb)(Packet) : callbacks[packet.id]) {
        cb(packet);
    }
}
