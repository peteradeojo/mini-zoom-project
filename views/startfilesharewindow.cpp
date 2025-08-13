#include "startfilesharewindow.h"
#include "ui_startfilesharewindow.h"

#include <QPushButton>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QIODevice>
#include <QDebug>

StartFileShareWindow::StartFileShareWindow(QWidget *parent, MiniZoom::AppServer *appserver)
    : QMainWindow(parent)
    , ui(new Ui::StartFileShareWindow)
{
    ui->setupUi(this);

    server = appserver;

    bool started = server->startServer(4068, [this](socket_t client_fd) {
        const QByteArray data = NULL;
        handleIncomingFile(client_fd, data);
    });
    if (started) {
        server->connectToServer("127.0.0.1", 4068);
    } else {
        this->close();
    }

    connect(ui->selectFileButton, &QPushButton::clicked, this, &StartFileShareWindow::onSelectFile);
}

StartFileShareWindow::~StartFileShareWindow()
{
    delete ui;
}

void StartFileShareWindow::sendFileToClients(const QString &filePath) {
    qDebug() << filePath;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        perror("error");
        return;
    }

    QFileInfo info(filePath);
    qDebug() << info;
    QByteArray header = QString("FILE-OPEN:%1:%2")
                            .arg(info.fileName())
                            .arg(file.size())
                            .toUtf8();

    int c;
    if (server->outgoingConnections.empty()) {
        server->broadcastMessage(header.constData());
    } else {
        c = server->outgoingConnections[0].socket_fd;
        server->broadcastMessage(header.constData(), c);
    }

    QByteArray buffer;
    if (server->outgoingConnections.empty()) {
        while (!(buffer = file.read(4096)).isEmpty()) {
            server->broadcastMessage(buffer.constData());
        }
    } else {
        while (!(buffer = file.read(4096)).isEmpty()) {
            server->broadcastMessage(buffer.constData(), c);
        }
    }

    server->broadcastMessage("FILE:END", c);
}

void StartFileShareWindow::handleIncomingFile(socket_t client_fd, const QByteArray &data) {
    qDebug() << "Received data from client" << client_fd;
}

void StartFileShareWindow::onSelectFile() {
    QString filePath = QFileDialog::getOpenFileName(this, "Select File to Send");
    if (filePath.isEmpty()) return;
    sendFileToClients(filePath);
}
