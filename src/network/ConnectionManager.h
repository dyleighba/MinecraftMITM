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
#include <stdlib.h>
#include <stdio.h>
#include <stdexcept>
#include <string>
#include "Connection.h"

class ConnectionManager {
private:
    bool wsaInitialized = false;
    void initWSA();
    void cleanupWSA();
    static void setNonBlocking(SOCKET &sock);
public:
    ConnectionManager();
    ~ConnectionManager();
    Connection connectToServer(PCSTR host, PCSTR port);
    Connection waitForClientConnection(PCSTR bindAddr, PCSTR bindPort);
};


#endif //MCPROXY_CONNECTIONMANAGER_H
