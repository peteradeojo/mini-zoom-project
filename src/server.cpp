#include <thread>

#include "server.h"

#include <QDebug>

namespace MiniZoom {

void AppServer::broadcastMessage(const std::string &msg) {
    for (auto &c : clients) {
            int i = send(c.socket_fd, msg.c_str(), (int)msg.size(), 0);
            if (i == SOCKET_ERROR) {
#ifdef _WIN32
                std::cerr << "Send Error: " << WSAGetLastError() << "\n";
#else
                perror("Send error: ");
#endif
            }
    }
}

void AppServer::broadcastMessage(const std::string &msg, socket_t sender) {
    qDebug() << msg;
    for (auto &c : clients) {
        if (c.socket_fd != sender && c.status != -1) {
            int i = send(c.socket_fd, msg.c_str(), (int)msg.size(), 0);
            if (i == SOCKET_ERROR) {
#ifdef _WIN32
                std::cerr << "Send Error: " << WSAGetLastError() << "\n";
#else
                perror("Send error: ");
#endif
            }
        }
    }
}

bool AppServer::connectToServer(const std::string &ip, int port) {
    socket_t sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET_FD) {
#ifdef _WIN32
        std::cerr << "socket failed: " << WSAGetLastError() << "\n";
#else
        perror("Socket error: ");
#endif
        return false;
    }

    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address\n";
        ::CLOSE_SOCKET(sock);
        return false;
    }

    if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
#ifdef _WIN32
        std::cerr << "connect failed: " << WSAGetLastError() << "\n";
#else
        perror("Connect error: ");
#endif
        ::CLOSE_SOCKET(sock);
        return false;
    }

    CxClient conn{sock, 1};
    clients.push_back(conn);
    outgoingConnections.push_back(conn);
    std::cout << "Connected to server at " << ip << ":" << port << "\n";
    return true;
}

void AppServer::addHandler(ClientHandler handler) {
    clientHandler = handler;

    CxClient conn = outgoingConnections[0];
    std::thread(clientHandler, conn.socket_fd).detach();
}
}
