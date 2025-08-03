#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>

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

private:
    Ui::ServerWindow *ui;
    void setupSocket();
    int createServer();
};

#endif // SERVERWINDOW_H
