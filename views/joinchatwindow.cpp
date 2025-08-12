#include "joinchatwindow.h"
#include "ui_joinchatwindow.h"

#include <QDebug>
#include <QLineEdit>
#include <QString>
#include <QByteArray>

JoinChatWindow::JoinChatWindow(QWidget *parent, MiniZoom::AppServer *appserver)
    : QMainWindow(parent)
    , ui(new Ui::JoinChatWindow)
{
    ui->setupUi(this);

    server = appserver;

    if (server) {
        MiniZoom::ClientHandler h = [this, parent](socket_t socket_fd) {
            char buffer[1024];

            while (true) {
                ssize_t bytes = recv(socket_fd, buffer, sizeof(buffer), 0);
                qDebug() << "Received " << bytes << " bytes of data";
                if (bytes <= 0) {
                    perror("Receive error");
                    ::CLOSE_SOCKET(socket_fd);
                    break;
                }

                buffer[bytes] = '\0';
                QString msg = QString::fromUtf8(buffer);
                qDebug() << "[Received]:" << buffer;
                emit messageReceived(msg);
            }

            this->hide();
            parent->show();
        };

        server->connectToServer("127.0.0.1", 4068);
        server->addHandler(h);
    }

    connect(this, &JoinChatWindow::messageReceived, this, &JoinChatWindow::displayMessage);
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &JoinChatWindow::on_inputReceived);
}

JoinChatWindow::~JoinChatWindow()
{
    delete ui;
}

void JoinChatWindow::displayMessage(QString msg) {
    QString formatted = QString("<span style='color:blue; font-weight:bold;'>%1</span>")
                            .arg(msg.toHtmlEscaped());
    ui->textBrowser->append(formatted);
}

void JoinChatWindow::on_inputReceived()
{
    QString input = ui->lineEdit->text();

    int c = server->outgoingConnections[0].socket_fd;

    QString msg = QString("[User: %1] %2").arg(c).arg(input);
    QByteArray send = msg.toUtf8();
    server->broadcastMessage(send.constData());

    displayMessage(send);

    ui->lineEdit->clear();
}
