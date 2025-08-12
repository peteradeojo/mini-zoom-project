#ifndef STARTFILESHAREWINDOW_H
#define STARTFILESHAREWINDOW_H

#include <QMainWindow>
#include <QByteArray>

#include "../src/server.h"

namespace Ui {
class StartFileShareWindow;
}

class StartFileShareWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartFileShareWindow(QWidget *parent = nullptr, MiniZoom::AppServer *appserver = NULL);
    ~StartFileShareWindow();

private:
    Ui::StartFileShareWindow *ui;
    MiniZoom::AppServer *server;
    void sendFileToClients(const QString &filePath);
    void handleIncomingFile(socket_t client_fd, const QByteArray &data);

private slots:
    void onSelectFile();
};

#endif // STARTFILESHAREWINDOW_H
