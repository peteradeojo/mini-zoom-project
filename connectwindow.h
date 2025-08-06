#ifndef CONNECTWINDOW_H
#define CONNECTWINDOW_H

#include <QMainWindow>

#include "src/socket_helper.h"

namespace Ui {
class ConnectWindow;
}

class ConnectWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConnectWindow(QWidget *parent = nullptr);
    ~ConnectWindow();
    void connectToServer();

private slots:
    void on_inputReceived();

private:
    Ui::ConnectWindow *ui;
    socket_t client_sock = INVALID_SOCKET_FD;
    void handleMessage();
};

#endif // CONNECTWINDOW_H
