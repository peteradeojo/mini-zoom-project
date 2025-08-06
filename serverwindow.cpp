#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "src/server.h"

#define MYPORT 4068

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerWindow)
    , timer(new QTimer(this))
{
    ui->setupUi(this);

    server = new MiniZoom::AppServer();
    if (server->createServer() > -1) {
        ui->status_label->setText("Server running");
    } else {
        perror("Error: ");
        ui->status_label->setText("Server initialization failed.");
    }

    connect(timer, &QTimer::timeout, this, &ServerWindow::updateClients);
    // timer->start(10); // refresh 10s
}

ServerWindow::~ServerWindow()
{
    delete ui;
    server->closeServer();
}

void ServerWindow::updateClients() {
    char text[64];
    int i = sprintf(text, "Connected clients: %d", server->getConnectedClients());
    if (i <= 0) {
        perror("Error: ");
    }

    printf("%s\n", text); // << std::endl;
    ui->status_label->setText(text);
}
