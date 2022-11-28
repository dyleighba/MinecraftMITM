//
// Created by Dylan on 11/13/2022.
//

#include "ConnectionManager.h"

void ConnectionManager::initWSA() {
    if (!wsaInitialized) {
        WSADATA wsaData;
        int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            printf("WSAStartup failed with error: %d\n", iResult);
            throw std::runtime_error("WSAStartup failed.");
        }
        wsaInitialized = true;
    }
}

void ConnectionManager::cleanupWSA() {
    WSACleanup();
}

void ConnectionManager::setNonBlocking(SOCKET &sock) {
    // Set as non-blocking
    u_long iMode = 1;
    int iResult = ioctlsocket(sock, FIONBIO, &iMode);
    if (iResult != NO_ERROR) {
        printf("ioctlsocket failed with error: %d\n", iResult);
        throw std::runtime_error("ioctlsocket failed");
    }
}

ConnectionManager::ConnectionManager() {
    initWSA();
}

ConnectionManager::~ConnectionManager() {
    cleanupWSA();
}

Connection ConnectionManager::connectToServer(PCSTR host, PCSTR port) {
    Connection conn = Connection();
    struct addrinfo *result = NULL;
    struct addrinfo hints{};
    int iResult;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    iResult = getaddrinfo( host, port, &hints, &result);
    if ( iResult != 0 ) {
        WSACleanup();
        throw std::runtime_error("getaddrinfo failed.");
    }
    // Create a SOCKET for the server to listen for client connections.
    conn.sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (conn.sock == INVALID_SOCKET) {
        freeaddrinfo(result);
        WSACleanup();
        throw std::runtime_error("socket creation failed.");
    }
    freeaddrinfo(result);

    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr(host);
    clientService.sin_port = htons(std::stoi(port));

    //----------------------
    // Connect to server.
    iResult = connect(conn.sock, (SOCKADDR *) & clientService, sizeof (clientService));
    if (iResult == SOCKET_ERROR) {
        wprintf(L"connect function failed with error: %ld\n", WSAGetLastError());
        iResult = closesocket(conn.sock);
        if (iResult == SOCKET_ERROR)
            wprintf(L"closesocket function failed with error: %ld\n", WSAGetLastError());
        throw std::runtime_error("closesocket function failed");
    }
    wprintf(L"Connected to server.\n");

    setNonBlocking(conn.sock);
    return conn;
}

Connection ConnectionManager::waitForClientConnection(PCSTR bindAddr, PCSTR bindPort) {
    struct addrinfo *result = nullptr;
    struct addrinfo hints{};
    int iResult;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    // Resolve the server address and port
    iResult = getaddrinfo( bindAddr, bindPort, &hints, &result);
    if ( iResult != 0 ) {
        WSACleanup();
        throw std::runtime_error("getaddrinfo failed.");
    }
    // Create a SOCKET for the server to listen for client connections.
    SOCKET listenSock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSock == INVALID_SOCKET) {;
        freeaddrinfo(result);
        WSACleanup();
        throw std::runtime_error("socket creation failed.");
    }
    // Setup the TCP listening socket
    iResult = bind( listenSock, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        freeaddrinfo(result);
        closesocket(listenSock);
        WSACleanup();
        throw std::runtime_error("bind failed");
    }
    freeaddrinfo(result);
    // Start listening
    iResult = listen(listenSock, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenSock);
        WSACleanup();
        throw std::runtime_error("listen failed");
    }

    Connection conn = Connection();
    // Accept a client socket
    conn.sock = accept(listenSock, NULL, NULL);
    if (conn.sock == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(listenSock);
        WSACleanup();
        throw std::runtime_error("accept failed");
    }
    setNonBlocking(conn.sock);
    return conn;
}
