#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <string>
#include <mutex>
//server.h defines generic server implementation
#include "socket_helper.h"

#define MAX_CLIENTS 4

namespace MiniZoom{

struct CxClient {
    socket_t socket_fd;
    short int status;
};

class AppServer {
public:
    AppServer();
    ~AppServer();
    int createServer();
    int closeServer();
    short int getConnectedClients() const;
    void handleClient(socket_t client_sock);
    void removeClient(int sock);
    int createClient(const std::string &serverIP, const std::string &serverPort);
    int createClient(const std::string &serverIP, const int serverPort);
    // int clients[MAX_CLIENTS] = {};

    // CxClient* root_client;
    std::vector<CxClient> clients;
    std::mutex clients_mutex;

private:
    socket_t server_sock = -1;
    short int connected_clients = 0;
    void startAcceptLoop();
    void broadcastMessage(const std::string msg, socket_t sender);
};
}


#endif // SERVER_H
