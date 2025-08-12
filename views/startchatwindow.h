#ifndef STARTCHATWINDOW_H
#define STARTCHATWINDOW_H

#include <QMainWindow>
#include <QString>

#include "../src/server.h"

namespace Ui {
class StartChatWindow;
}

class StartChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit StartChatWindow(QWidget *parent = nullptr, MiniZoom::AppServer *appserver = NULL);
    ~StartChatWindow();

private:
    Ui::StartChatWindow *ui;
    MiniZoom::AppServer *server;

signals:
    void messageReceived(QString message);

private slots:
    void on_inputReceived();
    void displayMessage(QString message);
};

#endif // STARTCHATWINDOW_H
