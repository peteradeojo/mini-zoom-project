#ifndef SOCKET_HELPER_H
#define SOCKET_HELPER_H

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")

typedef SOCKET socket_t;
#define INVALID_SOCKET_FD INVALID_SOCKET
#define CLOSE_SOCKET(s) closesocket(s)

#else // Linux / POSIX
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

typedef int socket_t;
#define INVALID_SOCKET_FD (-1)
#define CLOSE_SOCKET(s) close(s)
#endif

#include <iostream>

namespace socket_helper {

#ifdef _WIN32
class WinsockInit {
public:
    WinsockInit() {
        WSAData wsaData;
        int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (result != 0) {
            std::cerr << "WSAStartup failed with error: " << result << std::endl;
        }
    }

    ~WinsockInit() {
        WSACleanup();
    }
};
#endif

// Create a TCP socket
inline socket_t create_socket() {
    socket_t sock = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET_FD) {
#ifdef _WIN32
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
#else
        std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
#endif
    }
    return sock;
}

} // namespace socket_helper

#endif // SOCKET_HELPER_H
