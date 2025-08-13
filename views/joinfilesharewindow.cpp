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
    client->connectToServer("127.0.0.1", 4068);

    client->addHandler([this](socket_t client_sock){
        char buffer[4096];

        memset(buffer, 0, sizeof(buffer));
        while (true) {
            int iRes = recv(client_sock, buffer, sizeof(buffer), 0);
            if (iRes <= 0) {
                perror("receive error");
                continue;
            }

            QByteArray data = QByteArray::fromRawData(buffer, iRes);
            emit dataReceived(data);
        }
    });
}

JoinFileShareWindow::~JoinFileShareWindow()
{
    delete ui;
}

void JoinFileShareWindow::onDataReceived(const QByteArray data) {
    static QFile file;
    static qint64 remaining = 0;
    qDebug() << data;

    if (data.startsWith("FILE-OPEN")) {
        // Parse header
        QList<QByteArray> parts = data.split(':');
        QString filename = parts[1];
        remaining = parts[2].toLongLong();
        file.setFileName(filename);

        qDebug() << "File name: " << parts[1];
        file.open(QIODevice::WriteOnly);
    } else {
        file.write(data);
        remaining -= data.size();
        qDebug() << "Data: " << data;
        qDebug() << "Remaining: " << remaining;
        // if (remaining <= 0) {
        //     file.close();
        //     qDebug() << "File received!";
        // }
    }
}
