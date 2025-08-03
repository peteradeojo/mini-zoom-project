#include "serverwindow.h"
#include "ui_serverwindow.h"
#include <iostream>

#define MYPORT 4068

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
    // setupSocket();
    createServer();
}

ServerWindow::~ServerWindow()
{
    delete ui;
    if (server_sock != INVALID_SOCKET_FD || server_sock != INVALID_SOCKET)
        ::CLOSE_SOCKET(server_sock);

    WSACleanup();
}

void ServerWindow::setupSocket() {
    server_sock = socket_helper::create_socket();
    if (server_sock == INVALID_SOCKET_FD) {
        std::cerr << "Failed to create socket";
    } else {
        QString str = "Socket created succesfully.";
        ui->status_label->setText(str);
    }

    struct sockaddr_in my_addr;
}
