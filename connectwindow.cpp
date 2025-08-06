#include "connectwindow.h"
#include "ui_connectwindow.h"

#include <string.h>
#include <thread>

ConnectWindow::ConnectWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ConnectWindow)
{
    ui->setupUi(this);

    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &ConnectWindow::on_inputReceived);

    connectToServer();
}

ConnectWindow::~ConnectWindow()
{
    WSACleanup();
    delete ui;
}

void ConnectWindow::on_inputReceived() {
    QString input = ui->lineEdit->text();
    if (input.length() < 1) {
        return;
    }

    wchar_t sendbuf[1024];
    input.toWCharArray(sendbuf);

    qDebug() << "Input:" << input;
    if (client_sock != INVALID_SOCKET) {
        int iResult = send(client_sock, (const char*) sendbuf, (int) strlen((const char *)sendbuf), 0);
        if (iResult == SOCKET_ERROR) {
            qDebug() << "Send error";
            // qDebug() << "send failed" << WSAGetLastError() << "\n";
            ::CLOSE_SOCKET(client_sock);
            WSACleanup();
            return;
        }
    }
    ui->lineEdit->clear();
}

void ConnectWindow::connectToServer() {
    WSADATA wsa;
    int iResult;

    if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
        perror("Error: ");
        qDebug() << "WSAStartup failed\n";
        return;
    }

    struct addrinfo hints, *ptr = NULL, *result = NULL;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    if(getaddrinfo(NULL, "4068", &hints, &result) != 0) {
        perror("getaddr failed: ");
        WSACleanup();
        return;
    }

    ptr=result;

    // Follow the linked list to get the right address info
    do {
        client_sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (client_sock == INVALID_SOCKET_FD) {
            perror("Socket error: ");
            // freeaddrinfo(result);
            // WSACleanup();
            // return;
        }


        iResult = ::connect(client_sock, ptr->ai_addr, (int) ptr->ai_addrlen);

        if (iResult == SOCKET_ERROR) {
            qDebug() << "Unable to connect to server" << WSAGetLastError();
            perror("Error: ");
        } else {
            break;
        }

        ptr=ptr->ai_next;
    } while (ptr != NULL);

    if (client_sock == INVALID_SOCKET) {
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    std::thread([this]() {}).detach();

    qDebug() << "Connected to server successfully";
}

void ConnectWindow::handleMessage() {}
