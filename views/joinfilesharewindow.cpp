#include "joinfilesharewindow.h"
#include "ui_joinfilesharewindow.h"

#include <QDebug>
#include <QFile>

JoinFileShareWindow::JoinFileShareWindow(QWidget *parent, MiniZoom::AppServer *a_client)
    : QMainWindow(parent)
    , ui(new Ui::JoinFileShareWindow)
{
    ui->setupUi(this);

    connect(this, &JoinFileShareWindow::dataReceived, this, &JoinFileShareWindow::onDataReceived);

    client = a_client;
    client->connectToServer("127.0.0.1", 4545);

    client->addHandler([this](socket_t client_sock){
        char buffer[4096];
        while (true) {
            int iRes = recv(client_sock, buffer, sizeof(buffer), 0);
            if (iRes <= 0) {
                perror("receive error");
                continue;
            }

            qDebug() << buffer;

            QByteArray data = QByteArray::fromRawData(buffer, iRes);
            emit dataReceived(data);
        }
    });
}

JoinFileShareWindow::~JoinFileShareWindow()
{
    delete ui;
}

void JoinFileShareWindow::onDataReceived(const QByteArray &data) {
    static QFile file;
    static qint64 remaining = 0;

    if (data.startsWith("FILE:")) {
        // Parse header
        QList<QByteArray> parts = data.split(':');
        QString filename = parts[1];
        remaining = parts[2].toLongLong();
        file.setFileName(filename);
        file.open(QIODevice::WriteOnly);
    } else {
        file.write(data);
        remaining -= data.size();
        if (remaining <= 0) {
            file.close();
            qDebug() << "File received!";
        }
    }
}
