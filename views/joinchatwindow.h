#ifndef JOINCHATWINDOW_H
#define JOINCHATWINDOW_H

#include <QMainWindow>
#include <QString>

#include "../src/server.h"

namespace Ui {
class JoinChatWindow;
}

class JoinChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit JoinChatWindow(QWidget *parent = nullptr, MiniZoom::AppServer *appserver = NULL);
    ~JoinChatWindow();

private:
    Ui::JoinChatWindow *ui;
    MiniZoom::AppServer *server;

private slots:
    void displayMessage(QString msg);
    void on_inputReceived();

signals:
    void messageReceived(QString msg);
};

#endif // JOINCHATWINDOW_H
