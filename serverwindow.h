#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include "src/socket_helper.h"

#include "src/server.h"

namespace Ui {
class ServerWindow;
}

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();

private slots:
    void updateClients();

private:
    int connected_clients = 0;
    QTimer *timer;
    Ui::ServerWindow *ui;
    MiniZoom::AppServer *server;
};

#endif // SERVERWINDOW_H
