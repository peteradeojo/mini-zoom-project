#include "server.h"

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

void AppServer::handleClient(socket_t client_sock) {
    char buffer[1024];
    while (true) {
        int bytesReceived = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) break;

        buffer[bytesReceived] = '\0';
        printf("Received: %s\n", buffer);

        // Strip trailing \r and \n
        while (bytesReceived > 0 && (buffer[bytesReceived - 1] == '\n' || buffer[bytesReceived - 1] == '\r')) {
            buffer[--bytesReceived] = '\0';
        }

        if (strcmp(buffer, "end") == 0) break;

        send(client_sock, buffer, bytesReceived, 0);
    }

    CLOSE_SOCKET(client_sock);
    connected_clients--;
}
}
