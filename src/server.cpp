#include "server.h"

#include <iostream>
#include <vector>
#include <string>
// #include <thread>
// #include <algorithm>

namespace MiniZoom {
AppServer::AppServer() {
    // createServer();
}

AppServer::~AppServer() {
    closeServer();
}

short int AppServer::getConnectedClients() const {
    return connected_clients;
}

void AppServer::removeClient(int sock) {
    clients.erase(
        std::remove_if(clients.begin(), clients.end(),
                       [sock](const CxClient &c) { return c.socket_fd == sock; }),
        clients.end()
        );
}

void AppServer::handleClient(socket_t client_sock) {
    char buffer[1024];
    while (true) {
        int bytesReceived = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) break;

        printf("Received [%d]: %s\n", bytesReceived, buffer);

        // Strip trailing \r and \n
        while (bytesReceived > 0 && (buffer[bytesReceived - 1] == '\n' || buffer[bytesReceived - 1] == '\r')) {
            buffer[--bytesReceived] = '\0';
        }

        if (strcmp(buffer, "end") == 0) break;
        broadcastMessage(buffer, client_sock);
    }

    CLOSE_SOCKET(client_sock);
    connected_clients--;
}

void AppServer::broadcastMessage(const std::string msg, socket_t sender) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (auto it = clients.begin(); it != clients.end(); ++it) {
        if (it->socket_fd != sender && it->status != -1) {
            // int sent = send(it->socket_fd, msg.c_str(), msg.size(), 0);
            // if (sent == -1) {
            //     std::cerr << "Send failed to socket " << it->socket_fd << "\n";
            //     it->status = -1;
            // }
            std::cout << "Sending broadcast message";
        }
    }
}
}
