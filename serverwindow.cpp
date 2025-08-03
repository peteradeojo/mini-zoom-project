#include "serverwindow.h"
#include "ui_serverwindow.h"
#include <iostream>

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerWindow)
{
    ui->setupUi(this);
    setupServer();
}

ServerWindow::~ServerWindow()
{
    delete ui;
    if (server_sock != INVALID_SOCKET_FD)
        ::CLOSE_SOCKET(server_sock);
}

void ServerWindow::setupServer() {
    server_sock = socket_helper::create_socket();
    if (server_sock == INVALID_SOCKET_FD) {
        std::cerr << "Failed to create socket";
    } else {
        QString str = "Socket created succesfully.";
        ui->status_label->setText(str);
    }

    struct sockaddr_in my_addr;

    // my_addr.sin_addr=

    // bind(server_sock, )
}
