#include "connectwindow.h"
#include "ui_connectwindow.h"

#include <string.h>
#include <thread>

ConnectWindow::ConnectWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ConnectWindow)
{
    ui->setupUi(this);

    // connect(ui->lineEdit, &QLineEdit::returnPressed, this, &ConnectWindow::on_inputReceived);

    // connectToServer();
}

ConnectWindow::~ConnectWindow()
{
    delete ui;
}

void ConnectWindow::on_inputReceived() {
    QString input = ui->lineEdit->text();
    if (input.isEmpty()) {
        return;
    }

    qDebug() << "Input:" << input;
    if (client_sock != INVALID_SOCKET) {
        QByteArray data = input.toUtf8();
        int iResult = send(client_sock, data.constData(), data.size(), 0);
        if (iResult == SOCKET_ERROR) {
            qDebug() << "send failed" << WSAGetLastError() << "\n";
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

    char node[64] = "127.0.0.1";
    if(getaddrinfo(node, "4068", &hints, &result) != 0) {
        perror("getaddr failed: ");
        WSACleanup();
        return;
    }

    ptr=result;

    // Follow the linked list to get the right address info
    do {
        client_sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (client_sock == INVALID_SOCKET_FD) {
            std::cerr << "Socket error: " << WSAGetLastError() << "\n";
            break;
        }

        iResult = ::connect(client_sock, ptr->ai_addr, (int) ptr->ai_addrlen);

        if (iResult == SOCKET_ERROR) {
            qDebug() << "Unable to connect to server:" << WSAGetLastError();
            ::CLOSE_SOCKET(client_sock);
            client_sock = INVALID_SOCKET_FD;
            perror("Connect error: ");
        } else {
            break;
        }

        ptr=ptr->ai_next;
    } while (ptr != NULL);

    if (client_sock == INVALID_SOCKET) {
        qDebug() << "Unable to successfully establish a connection to the server";
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    std::thread([this]() {
        this->handleMessage();
        WSACleanup();
    }).detach();

    // Get remote server details
    sockaddr_storage remote_addr;
    int addr_len = sizeof(remote_addr);
    int port = 0;
    iResult = getpeername(client_sock, (sockaddr*)&remote_addr, &addr_len) == 0;
    if (iResult == SOCKET_ERROR) {
        perror("getpeername failed");
        qDebug() << "Unable to connect to server" << WSAGetLastError();
        ::CLOSE_SOCKET(client_sock);
        WSACleanup();
        return;
    }

    char ipstr[INET6_ADDRSTRLEN];

    if (remote_addr.ss_family == AF_INET) {
        sockaddr_in *s = (sockaddr_in *)&remote_addr;
        inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof(ipstr));
        port = ntohs(s->sin_port);
    } else if (remote_addr.ss_family == AF_INET6) {
        sockaddr_in6 *s = (sockaddr_in6 *)&remote_addr;
        inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof(ipstr));
        port = ntohs(s->sin6_port);
    }

    qDebug() << "Connected to server at IP:" << ipstr << "Port:" << port;
}

void ConnectWindow::handleMessage() {
    char buf[1024];
    while (true) {
        int resultBytes = recv(client_sock, buf, sizeof(buf) - 1, 0);
        if (resultBytes <= 0) break;

        buf[resultBytes] = '\0';
        qDebug() << "Received: " << buf;

        if (strcmp(buf, "end") == 0) break;
    }

    ::CLOSE_SOCKET(client_sock);
    WSACleanup();
}
