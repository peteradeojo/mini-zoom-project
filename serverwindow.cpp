#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "src/server.h"

#define MYPORT 4068

ServerWindow::ServerWindow(MiniZoom::AppServer *appserver, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerWindow)
    , timer(new QTimer(this))
{
    ui->setupUi(this);

    server = appserver;

    MiniZoom::ClientHandler h = [this](socket_t clientSock){
        char buffer[1024];
        int bufferSize = sizeof(buffer);

        ZeroMemory(buffer, bufferSize);

        while (true) {
            int s = recv(clientSock, buffer, bufferSize, 0);
            if (s <= 0) {
                std::cerr << "Receive error: " << WSAGetLastError() << "\n";
                CLOSE_SOCKET(clientSock);
                WSACleanup();
                break;
            }

            std::cout << "[Client]: " << buffer << "\n";
            server->broadcastMessage(buffer, clientSock);
        }

        CLOSE_SOCKET(clientSock);
    };

    if (server->startServer(MYPORT, h) == true) {
        server->connectToServer("127.0.0.1", MYPORT);
        ui->status_label->setText("Server running");
    } else {
        perror("Error: ");
        ui->status_label->setText("Server initialization failed.");
    }

    // connect(timer, &QTimer::timeout, this, &ServerWindow::updateClients);
    // timer->start(10); // refresh 10s
}

ServerWindow::~ServerWindow()
{
    if (server) {
        server->stopServer();
    }
    delete ui;
    delete server;
}

