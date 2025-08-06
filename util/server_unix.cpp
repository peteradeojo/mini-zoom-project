#include <iostream>
#include <thread>
#include "../src/server.h"

#define MYPORT 4068

namespace MiniZoom {

int AppServer::createServer() {
    if (server_sock != INVALID_SOCKET_FD) return 1;

    struct sockaddr_in myaddr;

    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(MYPORT);
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    // myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    memset(&(myaddr.sin_zero), '\0', 8);

    // get socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == INVALID_SOCKET_FD) {
        perror("An error occurred: ");
        return -1;
    }

    int opt = 1;
    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        ::close(server_sock);
        return -1;
    }

    // bind
    int bindResult = bind(server_sock, (struct sockaddr *)&myaddr, sizeof(sockaddr));
    if (bindResult == -1) {
        perror("Error:");
        return -1;
    }

    printf("Got past binding stage \n");
    // listen
    if (listen(server_sock, 4) == -1) {
        perror("Error: ");
        return -1;
    }

    // start accepting in a mulithread
    std::thread([this]() {
        startAcceptLoop();
    }).detach();

    return 1;
}

int AppServer::closeServer() {
    ::close(server_sock);
    return 1;
}

void AppServer::startAcceptLoop() {
    try {
        printf("Starting accept loop\n");
        while (true) {
            sockaddr_in client_addr;
            socklen_t client_size = sizeof(client_addr);
            socket_t client_sock = accept((int) server_sock, (sockaddr*)&client_addr, &client_size);

            if (client_sock == INVALID_SOCKET_FD) {
                perror("Error occurred: ");
                continue;
            }

            printf("Connected a client\n");

            connected_clients++;

            // Spawn a new thread for each client
            std::thread([this, client_sock]() {
                this->handleClient(client_sock);
            }).detach();
        }
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << std::endl;
    }
}
}
