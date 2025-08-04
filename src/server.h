#ifndef SERVER_H
#define SERVER_H

//server.h defines generic server implementation
#include "socket_helper.h"

namespace MiniZoom{
class AppServer {
public:
    AppServer();
    ~AppServer();
    int createServer();
    int closeServer();
    short int getConnectedClients() const;
    void handleClient(socket_t client_sock);

private:
    socket_t server_sock = -1;
    short int connected_clients = 0;
    void startAcceptLoop();
};
}


#endif // SERVER_H
