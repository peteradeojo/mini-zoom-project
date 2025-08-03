#include "../serverwindow.h"
#include "../ui_serverwindow.h"

#define DEFAULT_PORT "27015"

struct addrinfo *result = NULL, *ptr = NULL, hints;

int ServerWindow::createServer() {
    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the local address and port to be used by the server
    int iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed: %d\n", iResult);
    }

    server_sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (server_sock == INVALID_SOCKET) {
        printf("Error at socket(): %d\n", WSAGetLastError());
        freeaddrinfo(result);
        return -1;
    }

    iResult = bind(server_sock, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        ::CLOSE_SOCKET(server_sock);
        return -1;
    }

    ui->status_label->setText("Server bind successful.");

    return 0;
}
