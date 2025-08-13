#ifndef JOINFILESHAREWINDOW_H
#define JOINFILESHAREWINDOW_H

#include <QMainWindow>
#include <QByteArray>

#include "../src/server.h"

namespace Ui {
class JoinFileShareWindow;
}

class JoinFileShareWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit JoinFileShareWindow(QWidget *parent = nullptr, MiniZoom::AppServer *a_client = NULL);
    ~JoinFileShareWindow();

private:
    Ui::JoinFileShareWindow *ui;
    MiniZoom::AppServer *client;

private slots:
    void onDataReceived(const QByteArray data);

signals:
    void dataReceived(const QByteArray data);
};

#endif // JOINFILESHAREWINDOW_H
