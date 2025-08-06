// #define DEFAULT_PORT "27015"

// struct addrinfo *result = NULL, *ptr = NULL, hints;

// int ServerWindow::createServer() {
//     WSADATA wsa;

//     if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
//         perror("Error: ");
//         std::cerr << "WSAStartup failed\n";
//         return -1;
//     }

//     // clear memory
//     ZeroMemory(&hints, sizeof (hints));
//     hints.ai_family = AF_INET;
//     hints.ai_socktype = SOCK_STREAM;
//     hints.ai_protocol = IPPROTO_TCP;
//     hints.ai_flags = AI_PASSIVE;

//     char node[64] = "127.0.0.1";

//     // Resolve the local address and port to be used by the server
//     int iResult = getaddrinfo(node, DEFAULT_PORT, &hints, &result);
//     if (iResult != 0) {
//         printf("getaddrinfo failed: %d\n", iResult);
//     }

//     server_sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

//     if (server_sock == INVALID_SOCKET) {
//         printf("Error at socket(): %d\n", WSAGetLastError());
//         freeaddrinfo(result);

//         return -1;
//     }

//     iResult = bind(server_sock, result->ai_addr, (int)result->ai_addrlen);
//     if (iResult == SOCKET_ERROR) {
//         printf("bind failed with error: %d\n", WSAGetLastError());
//         freeaddrinfo(result);
//         ::CLOSE_SOCKET(server_sock);

//         return -1;
//     }

//     if (listen(server_sock, 4) == SOCKET_ERROR) {
//         printf("Socket listen failed with error: %d\n", WSAGetLastError());
//         ::CLOSE_SOCKET(server_sock);

//         return -1;
//     }

//     sockaddr_in localAddr;
//     int addrLen = sizeof(localAddr);

//     if (getsockname(server_sock, (SOCKADDR*)&localAddr, &addrLen) == 0) {
//         char ipStr[INET_ADDRSTRLEN];
//         inet_ntop(AF_INET, &(localAddr.sin_addr), ipStr, sizeof(ipStr));

//         int port = ntohs(localAddr.sin_port);

//         ui->status_label->setText(QString("Listening on %1:%2").arg(ipStr).arg(port));
//     } else {
//         std::cerr << "getsockname failed: " << WSAGetLastError() << std::endl;

//         return -1;
//     }

//     std::thread([this]() {
//         startAcceptLoop();
//     }).detach();

//     return 0;
// }

// void handle_client(socket_t client_sock) {
//     char buffer[1024];

//     while(true) {
//         int bytesReceived = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
//         if (bytesReceived <= 0) break; // client disconnection or error

//         buffer[bytesReceived] = '\0';
//         printf("Recevied: %s\n", buffer);

//         // echo message back to client
//         send(client_sock, buffer, bytesReceived, 0);
//     }

//     ::CLOSE_SOCKET(client_sock);
//     printf("Client disconnected.\n");
// }

// void ServerWindow::startAcceptLoop() {
//     try {
//     while (true) {
//         std::cout << "Starting accept connections" << std::endl;
//         sockaddr_in client_addr;
//         int client_size = sizeof(client_addr);
//         socket_t client_sock = accept(server_sock, (sockaddr*)&client_addr, &client_size);
//         if (client_sock == INVALID_SOCKET) {
//             perror("Error occurred: ");
//             char str[1024];
//             sprintf(str, "Accept failed: %d\n", WSAGetLastError());

//             std::cout << str;
//             continue;
//         }

//         // Spawn a new thread for each client
//         std::thread t(handle_client, client_sock);
//         t.detach(); // detaches the thread so it cleans itself up
//     }
//     } catch (const std::exception& ex) {
//         std::cerr << "Exception in accept loop: " << ex.what() << std::endl;
//     }
// }
