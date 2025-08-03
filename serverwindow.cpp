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

    if (server_sock != -1)
        CLOSE_SOCKET(server_sock);
}

void ServerWindow::setupServer() {
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == -1) {
        std::cerr << "Failed to create socket";
    } else {
        QString str = "Socket created succesfully.";
        ui->status_label->setText(str);
    }
}
