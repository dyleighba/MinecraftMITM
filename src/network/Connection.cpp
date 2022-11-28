//
// Created by Dylan on 11/13/2022.
//

#include "Connection.h"

// TODO
// Add connection drop detection and handling

Connection::Connection() {

}

Connection::~Connection() {
    shutdown(sock, SD_SEND);
    closesocket(sock);
}

std::vector<unsigned char> Connection::receiveRawData() {
    return receiveRawData(DEFAULT_BUFFER_SIZE, false);
}

std::vector<unsigned char> Connection::receiveRawData(int bytesToRecv, bool peek) {
    lastRecvPartial = false;
    char buffer[bytesToRecv];
    ZeroMemory(buffer, bytesToRecv);
    int recvFlags = 0;
    if (peek) {
        recvFlags = MSG_PEEK;
    }
    int iResult = recv(sock, buffer, bytesToRecv, recvFlags);
    std::vector<unsigned char> data;

    if (iResult == SOCKET_ERROR) {
        int errorCode = WSAGetLastError();
        if (errorCode == WSAECONNRESET || errorCode == WSAECONNABORTED) {
            _isConnected = false;
            return data;
        }
        else if (errorCode == WSAEMSGSIZE) {
            lastRecvPartial = true;
        }
        else if (errorCode == WSAEWOULDBLOCK) {
            return data;
        }

    }
    if (iResult <= 0 ) {
        _isConnected = false;
        return data;
    }
    data.reserve(iResult);
    for (int i=0; i<iResult; i++) {
        data.push_back(static_cast<unsigned char>(buffer[i]));
    }
    return data;
}

void Connection::sendRawData(std::vector<unsigned char> &data) {
    send(sock, (char *)data.data(), static_cast<int>(data.size()), 0);
}

Packet Connection::receivePacket() {
    Packet packet{-1};
    int length = getNextPacketLength();
    if (length <= 0) return packet;
    int varIntLength = VarInt::toBytes(length).size();
    std::vector<unsigned char> rawPacket = receiveRawData(length+varIntLength, false);
    if (rawPacket.empty()) { return packet; }
    rawPacket.erase(rawPacket.begin(), rawPacket.begin()+varIntLength);
    packet.id = VarInt::fromBytes(rawPacket);
    int packetIdLength = VarInt::toBytes(packet.id).size();
    rawPacket.erase(rawPacket.begin(), rawPacket.begin()+packetIdLength);
    packet.data = rawPacket;
    return packet;
}

void Connection::sendPacket(Packet &packet) {
    std::vector<unsigned char> byteArr{};
    byteArr.reserve(packet.data.size()+10);

    std::vector<unsigned char> packetId = VarInt::toBytes(packet.id);
    byteArr.insert(byteArr.end(), packetId.begin(), packetId.end());
    byteArr.insert(byteArr.end(), packet.data.begin(), packet.data.end());

    std::vector<unsigned char> packetSize = VarInt::toBytes(static_cast<int>(byteArr.size()));
    byteArr.insert(byteArr.begin(), packetSize.begin(), packetSize.end());
    sendRawData(byteArr);
}

bool Connection::isConnected() {
    return _isConnected;
}

int Connection::getNextPacketLength() {
    std::vector<unsigned char> data = receiveRawData(5, true);
    if (data.empty()) return -1;
    return VarInt::fromBytes(data);
}
