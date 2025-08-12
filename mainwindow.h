#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QThread>
#include <QImage>
#include <QPixmap>
#include <QPushButton>
#include <opencv2/opencv.hpp>

#include "serverwindow.h"
#include "connectwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void captureFrame();
    void on_startServerButton_clicked();
    void connectButton_clicked();

private:
    Ui::MainWindow *ui;
    ServerWindow *serverWindow = nullptr; // new window instance
    ConnectWindow *connectWindow = nullptr;
    cv::VideoCapture cap;
    QTimer *timer;

    MiniZoom::AppServer* server;
    QThread *serverThread;

};
#endif // MAINWINDOW_H
