#include "startchatwindow.h"
#include "ui_startchatwindow.h"

#include <QPushButton>
#include <QDebug>
#include <QLineEdit>
#include <QString>
#include <QByteArray>
#include <string>

StartChatWindow::StartChatWindow(QWidget *parent, MiniZoom::AppServer *appserver)
    : QMainWindow(parent)
    , ui(new Ui::StartChatWindow)
{
    ui->setupUi(this);
    server = appserver;

    MiniZoom::ClientHandler h = [this](socket_t socket_fd) {
        char buffer[1024];

        while (true) {
            int bytes = recv(socket_fd, buffer, sizeof(buffer), 0);
            qDebug() << "Received " << bytes << " bytes of data";
            if (bytes <= 0) {
                perror("Receive error");
                continue;
            }

            buffer[bytes] = '\0';
            QString msg = QString::fromUtf8(buffer);
            qDebug() << "[Received]: " << buffer;
            emit messageReceived(msg);
        }
    };

    bool started = server->startServer(4068, h);
    if (started) {
        server->connectToServer("0.0.0.0", 4068);
    } else {
        perror("Server error: ");
    }

    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &StartChatWindow::on_inputReceived);
    connect(ui->sendMsgBtn, &QPushButton::clicked, this, &StartChatWindow::on_inputReceived);
    connect(this, &StartChatWindow::messageReceived, this, &StartChatWindow::displayMessage);
}

StartChatWindow::~StartChatWindow()
{
    if (server) {
        server->stopServer();
    }
    delete ui;
}

void StartChatWindow::on_inputReceived() {
    QString input = ui->lineEdit->text();

    int c = server->outgoingConnections[0].socket_fd;

    QString send = QString("[User %1] %2").arg(c).arg(input);
    QByteArray msg = send.toUtf8();
    server->broadcastMessage(msg.constData());

    ui->lineEdit->clear();
}

void StartChatWindow::displayMessage(QString msg) {
    qDebug() << msg;

    QString formatted = QString("<span style='color:blue; font-weight:bold;'>%1</span>")
                            .arg(msg.toHtmlEscaped());
    ui->textBrowser->append(formatted);
}
