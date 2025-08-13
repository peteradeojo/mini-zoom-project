#include "joinfilesharewindow.h"
#include "ui_joinfilesharewindow.h"

#include <QDebug>
#include <QFile>
#include <QString>
#include <QDir>
#include <QStandardPaths>

JoinFileShareWindow::JoinFileShareWindow(QWidget *parent, MiniZoom::AppServer *a_client)
    : QMainWindow(parent)
    , ui(new Ui::JoinFileShareWindow)
{
    ui->setupUi(this);

    connect(this, &JoinFileShareWindow::dataReceived, this, &JoinFileShareWindow::onDataReceived);
    connect(this, &JoinFileShareWindow::fileReceived, this, &JoinFileShareWindow::onFileReceived);

    client = a_client;
    client->connectToServer("127.0.0.1", 4068);

    client->addHandler([this](socket_t client_sock){
        char buffer[4096];

        memset(buffer, 0, sizeof(buffer));
        while (true) {
            int iRes = recv(client_sock, buffer, sizeof(buffer), 0);
            if (iRes <= 0) {
                perror("receive error");
                ::CLOSE_SOCKET(client_sock);
                break;
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
        QString originalFilename = QString::fromUtf8(parts[1]);
        remaining = parts[2].toLongLong();

        // Get the user's Downloads folder
        QString downloadsPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
        if (downloadsPath.isEmpty() || QDir(downloadsPath).exists() == false) {
            downloadsPath = QDir::homePath(); // fallback if no Downloads folder found
        }

        // Construct full path with original filename
        QString fullPath = QDir(downloadsPath).filePath(originalFilename);
        qDebug() << "Saving file to: " << fullPath;

        file.setFileName(fullPath);
        if (!file.open(QIODevice::WriteOnly)) {
            qWarning() << "Could not open file for writing:" << file.errorString();
            return;
        }
    } else if (data.compare("FILE:END") == 0) {
        file.close();
        emit fileReceived(file.fileName().toStdString());
        return;
    } else {
        file.write(data);
        remaining -= data.size();
        qDebug() << "Remaining: " << remaining;
    }
}

void JoinFileShareWindow::onFileReceived(const std::string filename) {
    qDebug() << "Successfully received: " << filename.c_str();

    QString update = QString("Received file: %1").arg(QString(filename.c_str()).toHtmlEscaped());
    ui->textBrowser->append(update);
}
