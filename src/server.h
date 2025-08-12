#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <vector>
#include <functional>
#include "socket_helper.h"

#define MAX_CLIENTS 4

namespace MiniZoom {
struct CxClient {
    socket_t socket_fd;
    int status;
    bool sendEnabled;
};

using ClientHandler = std::function<void(socket_t)>;

class AppServer{

public:
    AppServer();
    ~AppServer();
    bool startServer(int port, ClientHandler handler);
    void stopServer();

    void acceptClients();
    void broadcastMessage(const std::string &msg, socket_t sender);
    bool connectToServer(const std::string &ip, const int port);

private:
    socket_t server_fd;
    sockaddr_in server_addr;
    std::vector<CxClient> clients;
    std::vector<CxClient> outgoingConnections;
    ClientHandler clientHandler;
};
}

#endif
