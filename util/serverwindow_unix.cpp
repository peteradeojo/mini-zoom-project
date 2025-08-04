// #include <thread>
// #include <string>

// #include "../serverwindow.h"
// #include "../ui_serverwindow.h"

// #define MYPORT 27015

// void handle_client(socket_t client_sock) {
//     char buffer[1024];

//     while(true) {
//         int bytesReceived = recv(client_sock, buffer, sizeof(buffer) - 1, 0);
//         if (bytesReceived <= 0) break; // client disconnection or error

//         buffer[bytesReceived] = '\0';
//         printf("Received: %s\n", buffer);
//         printf("Ended: %d\n", strcmp(buffer, "end"));

//         // Strip trailing \r and \n
//         while (bytesReceived > 0 && (buffer[bytesReceived - 1] == '\n' || buffer[bytesReceived - 1] == '\r')) {
//             buffer[--bytesReceived] = '\0';
//         }

//         if (strcmp(buffer, "end") == 0) {
//             break;
//         }

//         // echo message back to client
//         send(client_sock, buffer, bytesReceived, 0);
//     }

//     ::CLOSE_SOCKET(client_sock);
//     printf("Client disconnected.\n");
// }

// int ServerWindow::createServer() {
//     struct sockaddr_in myaddr;

//     myaddr.sin_family = AF_INET;
//     myaddr.sin_port = htons(MYPORT);
//     // myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
//     myaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
//     memset(&(myaddr.sin_zero), '\0', 8);

//     // get socket
//     server_sock = socket(AF_INET, SOCK_STREAM, 0);
//     if (server_sock == INVALID_SOCKET_FD) {
//         perror("An error occurred: ");
//         return -1;
//     }

//     int opt = 1;
//     if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
//         perror("setsockopt failed");
//         ::close(server_sock);
//         return -1;
//     }

//     // bind
//     int bindResult = bind(server_sock, (struct sockaddr *)&myaddr, sizeof(sockaddr));
//     if (bindResult == -1) {
//         perror("Error:");
//         return -1;
//     }

//     // listen
//     if (listen(server_sock, 4) == -1) {
//         perror("Error: ");
//         return -1;
//     }

//     // start accepting in a mulithread
//     // thread t(handle_client, server_sock);
//     // t.detach();

//     std::thread([this]() {
//         startAcceptLoop();
//     }).detach();

//     ui->status_label->setText("Server bind success");
//     return 0;
// }

// void ServerWindow::startAcceptLoop() {
//     try {
//         while (true) {
//             sockaddr_in client_addr;
//             socklen_t client_size = sizeof(client_addr);
//             socket_t client_sock = accept((int) server_sock, (sockaddr*)&client_addr, &client_size);

//             if (client_sock == INVALID_SOCKET_FD) {
//                 perror("Error occurred: ");
//                 continue;
//             }

//             connected_clients++;

//             // Spawn a new thread for each client
//             std::thread t(handle_client, client_sock);
//             t.detach(); // detaches the thread so it cleans itself up
//         }
//     } catch (const std::exception& ex) {
//         std::cerr << ex.what() << std::endl;
//     }
// }
