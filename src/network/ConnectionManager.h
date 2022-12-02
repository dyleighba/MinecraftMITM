//
// Created by Dylan on 11/13/2022.
//

#ifndef MCPROXY_CONNECTIONMANAGER_H
#define MCPROXY_CONNECTIONMANAGER_H

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdlib>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <network/Connection.h>
#include <network/NetworkAddress.h>

class ConnectionManager {
private:
    bool wsaInitialized = false;
    void initWSA();
    void cleanupWSA();
    static void setNonBlocking(SOCKET &sock);
public:
    ConnectionManager();
    ~ConnectionManager();
    Connection connectToServer(NetworkAddress serverAddr);
    Connection waitForClientConnection(NetworkAddress bindAddr);
};


#endif //MCPROXY_CONNECTIONMANAGER_H
