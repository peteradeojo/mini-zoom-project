#ifndef SOCKET_LIB_H
#define SOCKET_LIB_H

// #include <QApplication>

#ifdef __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define CLOSE_SOCKET(s) close(s)
typedef int socket_t;
#elif _WIN32
#include <winsock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#define CLOSE_SOCKET(s) closesocket(s)
typedef SOCKET socket_t;
#endif

// QT_BEGIN_NAMESPACE
// namespace App {
// class PortableSocket;
// };
// QT_END_NAMESPACE

// class PortableSocket {
// public:
//     PortableSocket();
//     ~PortableSocket();

//     socket_t createSocket();
//     void closeSocket(socket_t sock);
// };

#endif // SOCKET_LIB_H
