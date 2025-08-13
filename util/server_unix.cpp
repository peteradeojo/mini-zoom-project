#include <iostream>
#include <thread>
#include "../src/server.h"

#define MYPORT 4068

namespace MiniZoom {
AppServer::AppServer(): server_fd(INVALID_SOCKET_FD) {}

AppServer::~AppServer() {}

bool AppServer::startServer(int port, ClientHandler handler) {
    clientHandler = handler;

    server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == INVALID_SOCKET_FD) {
        perror("Error: ");
        return false;
    }

    int iRes;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int optval = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    iRes = bind(server_fd, (sockaddr*)&server_addr, sizeof(server_addr));
    if (iRes == SOCKET_ERROR) {
        perror("Socket bind error: ");
        ::CLOSE_SOCKET(server_fd);
        return false;
    }

    iRes = listen(server_fd, MAX_CLIENTS);
    if (iRes == SOCKET_ERROR) {
        perror("Listen error");
        ::CLOSE_SOCKET(server_fd);
        return false;
    }

    std::thread(&AppServer::acceptClients, this).detach();
    return true;
}

void AppServer::stopServer() {
    ::CLOSE_SOCKET(server_fd);
}

void AppServer::acceptClients() {
    sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);

    while(true) {
        socket_t new_sock = accept(server_fd, (sockaddr*)&client_addr, &addrlen);
        if (new_sock != INVALID_SOCKET_FD) {
            CxClient client{new_sock, 1};
            clients.push_back(client);

            std::thread(clientHandler, new_sock).detach();

            char clientIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(client_addr.sin_addr), clientIP, INET_ADDRSTRLEN);
            std::cout << "New client connected: " << clientIP << "\n";
        }
    }
}

}
