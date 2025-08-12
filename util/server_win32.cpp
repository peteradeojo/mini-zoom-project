#include "../src/server.h"

#include <iostream>
#include <thread>

namespace MiniZoom {
AppServer::AppServer(): server_fd(INVALID_SOCKET_FD) {
    WSAData wsa;
    int res = WSAStartup(MAKEWORD(2,2), &wsa);
    if (res != 0) {
        std::cerr << "WSA Startup failed: " << res << "\n";
    }
}

AppServer::~AppServer() {
    stopServer();
    WSACleanup();
}

bool AppServer::startServer(int port, ClientHandler handler) {
    clientHandler = handler;
    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == INVALID_SOCKET_FD) {
        std::cerr << "Server socket creation error: " << WSAGetLastError() << std::endl;
        return false;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Server socket bind error: " << WSAGetLastError() << std::endl;
        CLOSE_SOCKET(server_fd);
        return false;
    }

    if (listen(server_fd, MAX_CLIENTS) == SOCKET_ERROR) {
        std::cerr << "listen error: " << WSAGetLastError() << std::endl;
        CLOSE_SOCKET(server_fd);
        return false;
    }

    std::thread(&AppServer::acceptClients, this).detach();

    std::cout << "Server listening on port " << port << "\n";
    return true;
}

void AppServer::stopServer() {
    for (auto &c : clients) {
        closesocket(c.socket_fd);
    }
    for (auto &c : outgoingConnections) {
        closesocket(c.socket_fd);
    }
    if (server_fd != INVALID_SOCKET) {
        closesocket(server_fd);
    }
    clients.clear();
    outgoingConnections.clear();
}

void AppServer::acceptClients() {
    sockaddr_in client_addr;
    int addrlen = sizeof(client_addr);

    while(true) {
        socket_t new_sock = accept(server_fd, (sockaddr*)&client_addr, &addrlen);
        if (new_sock != INVALID_SOCKET) {
            CxClient client{new_sock, 1};
            clients.push_back(client);

            std::thread(clientHandler, new_sock).detach();

            char clientIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(client_addr.sin_addr), clientIP, INET_ADDRSTRLEN);
            std::cout << "New client connected: " << clientIP << "\n";
        }
    }
}

void AppServer::broadcastMessage(const std::string &msg, socket_t sender) {
    for (auto &c : clients) {
        if (c.socket_fd != sender && c.status != -1) {
            int i = send(c.socket_fd, msg.c_str(), (int)msg.size(), 0);
            if (i == SOCKET_ERROR) {
                std::cerr << "Send Error: " << WSAGetLastError() << "\n";
            }
        }
    }
}

bool AppServer::connectToServer(const std::string &ip, int port) {
    socket_t sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET_FD) {
        std::cerr << "socket failed: " << WSAGetLastError() << "\n";
        return false;
    }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address\n";
        closesocket(sock);
        return false;
    }

    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        std::cerr << "connect failed: " << WSAGetLastError() << "\n";
        closesocket(sock);
        return false;
    }

    CxClient conn{sock, 1};
    outgoingConnections.push_back(conn);
    std::cout << "Connected to server at " << ip << ":" << port << "\n";
    return true;
}
}
