#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "src/socket_helper.h"

namespace Ui {
class ServerWindow;
}

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();
    socket_t server_sock = -1;

private slots:
    void updateClients();

private:
    int connected_clients = 0;
    QTimer *timer;
    Ui::ServerWindow *ui;
    int createServer();
    // void handle_client(socket_t sock);
    void startAcceptLoop();
};

#endif // SERVERWINDOW_H
