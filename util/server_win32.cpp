#include <iostream>
#include <thread>
#include "../src/server.h"

#define DEFAULT_PORT ("4068")

struct addrinfo *result = NULL, hints;

namespace MiniZoom {
int AppServer::createServer() {
    WSADATA wsa;

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        perror("Error: ");
        std::cerr << "WSAStartup failed\n";
        return -1;
    }

    // clear memory
    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    char node[64] = "127.0.0.1";

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

    if (listen(server_sock, MAX_CLIENTS) == SOCKET_ERROR) {
        printf("Socket listen failed with error: %d\n", WSAGetLastError());
        ::CLOSE_SOCKET(server_sock);

        return -1;
    }

    sockaddr_in localAddr;
    int addrLen = sizeof(localAddr);

    if (getsockname(server_sock, (SOCKADDR*)&localAddr, &addrLen) == 0) {
        char ipStr[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(localAddr.sin_addr), ipStr, sizeof(ipStr));

        int port = ntohs(localAddr.sin_port);
        printf("Listening on %s:%d", ipStr, port);
    } else {
        std::cerr << "getsockname failed: " << WSAGetLastError() << std::endl;
        return -1;
    }

    std::thread([this]() {
        startAcceptLoop();
    }).detach();

    return 0;
}

void AppServer::startAcceptLoop() {
    try {
        while (true) {
            std::cout << "Starting accept connections" << std::endl;
            sockaddr_in client_addr;
            int client_size = sizeof(client_addr);
            socket_t client_sock = accept(server_sock, (sockaddr*)&client_addr, &client_size);
            if (client_sock == INVALID_SOCKET) {
                perror("Error occurred: ");
                printf("Accept failed: %d\n", WSAGetLastError());
                continue;
            }

            // Spawn a new thread for each client
            // std::thread t(handle_client, client_sock);
            std::lock_guard<std::mutex> lock(clients_mutex);
            CxClient ci;
            ci.socket_fd = client_sock;
            clients.push_back(ci);

            std::thread([this, client_sock]() {
                this->handleClient(client_sock);
            }).detach();
        }
    } catch (const std::exception& ex) {
        std::cerr << "Exception in accept loop: " << ex.what() << std::endl;
    }
}

int AppServer::closeServer() {
    return 1;
}

int AppServer::createClient(const std::string &serverIP, const std::string &serverPort) {
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "WSAStartup failed\n";
        return -1;
    }

    struct addrinfo *addr_result = nullptr;
    struct addrinfo hints_client;
    ZeroMemory(&hints_client, sizeof(hints_client));
    hints_client.ai_family = AF_INET;
    hints_client.ai_socktype = SOCK_STREAM;
    hints_client.ai_protocol = IPPROTO_TCP;

    int iResult = getaddrinfo(serverIP.c_str(), serverPort.c_str(), &hints_client, &addr_result);
    if (iResult != 0) {
        std::cerr << "getaddrinfo failed: " << iResult << "\n";
        return -1;
    }

    socket_t clientSocket = socket(addr_result->ai_family, addr_result->ai_socktype, addr_result->ai_protocol);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Client socket creation failed: " << WSAGetLastError() << "\n";
        freeaddrinfo(addr_result);
        return -1;
    }

    iResult = connect(clientSocket, addr_result->ai_addr, (int)addr_result->ai_addrlen);
    freeaddrinfo(addr_result);

    if (iResult == SOCKET_ERROR) {
        std::cerr << "Unable to connect to server: " << WSAGetLastError() << "\n";
        CLOSE_SOCKET(clientSocket);
        return -1;
    }

    std::cout << "Connected to server at " << serverIP << ":" << serverPort << "[" << clientSocket << "]\n";

    CxClient ci;
    ci.socket_fd = clientSocket;
    clients.push_back(ci);

    // Optionally, start listening to incoming messages from this connection
    std::thread([clientSocket, this]() {
        char buffer[1024];
        while (true) {
            int bytes = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytes <= 0) {
                removeClient(clientSocket);
                break;
            }
            buffer[bytes] = '\0';
            std::cout << "[Server Reply]: " << buffer << "\n";
        }
        CLOSE_SOCKET(clientSocket);
    }).detach();

    return 0;
}
}
