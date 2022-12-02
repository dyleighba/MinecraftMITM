//
// Created by Dylan on 11/13/2022.
//

#ifndef MCPROXY_CONNECTION_H
#define MCPROXY_CONNECTION_H

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdlib>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>
#include "network/Packet.h"
#include "type/VarInt.h"

class Connection {
private:
    static const int DEFAULT_BUFFER_SIZE = 8192;
    bool _isConnected = true; // This will give an incorrect reading if the Connection isn't setup yet.
    bool lastRecvPartial = false;
public:
    Connection();
    ~Connection();
    SOCKET sock = INVALID_SOCKET;
    std::vector<unsigned char> receiveRawData(int bytesToRecv, bool peek);
    std::vector<unsigned char> receiveRawData();
    void sendRawData(std::vector<unsigned char> &data);
    bool hasValidSocket() { return sock != INVALID_SOCKET; };
    int getNextPacketLength();
    bool isConnected();
};


#endif //MCPROXY_CONNECTION_H
