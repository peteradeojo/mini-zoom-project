#include "serverwindow.h"
#include "ui_serverwindow.h"
#include <iostream>

#define MYPORT 4068

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerWindow)
    , timer(new QTimer(this))
{
    ui->setupUi(this);
    createServer();

    connect(timer, &QTimer::timeout, this, &ServerWindow::updateClients);
    // timer->start(10); // refresh 10s
}

ServerWindow::~ServerWindow()
{
    delete ui;

#ifdef _WIN32
    if (server_sock != INVALID_SOCKET)
        ::CLOSE_SOCKET(server_sock);
    WSACleanup();
#else
    // if (server_sock != INVALID_SOCKET_FD)
    ::CLOSE_SOCKET(server_sock);
#endif
}

void ServerWindow::updateClients() {
    char text[64];
    int i = sprintf(text, "Connected clients: %d", connected_clients);
    if (i <= 0) {
        perror("Error: ");
    }

    printf("%s\n", text); // << std::endl;
    ui->status_label->setText(text);
}
