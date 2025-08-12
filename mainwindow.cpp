#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timer(new QTimer(this))
    , server(nullptr)
    , serverThread(nullptr)
{
    ui->setupUi(this);

    // connect(timer, &QTimer::timeout, this, &MainWindow::captureFrame);
    // timer->start(30); // 30ms interval ~33fps

    connect(ui->initTextChat, &QPushButton::clicked, this, &MainWindow::on_startChat);
    connect(ui->joinTextChat, &QPushButton::clicked, this, &MainWindow::on_joinChat);
}

MainWindow::~MainWindow() {
    if (server) {
        server->stopServer();
        delete server;
    }

    if (serverThread) {
        serverThread->quit();
        serverThread->wait();
        delete serverThread;
    }
    delete ui;
}

void MainWindow::captureFrame() {
    cv::Mat frame;
    cap >> frame;

    if (frame.empty())
        return;

    // Convert to RGB
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

    // Convert to QImage
    QImage img((const uchar*)frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);

    // Scale image
    QPixmap pix = QPixmap::fromImage(img).scaled(
        // ui->label->size(),
        Qt::KeepAspectRatio,
        Qt::SmoothTransformation
        );

    // Show in QLabel
    // ui->label->setPixmap(pix);
}

void MainWindow::on_startChat() {
    this->hide();
    if (!start_chatWindow) {
        if (server == NULL) {
            server = new MiniZoom::AppServer();
        }
        start_chatWindow = new StartChatWindow(this, server);
        start_chatWindow->show();
    }
}

void MainWindow::on_joinChat() {
    this->hide();
    if (!join_chatWindow) {
        if (server == NULL) {
            server = new MiniZoom::AppServer();
        }
        join_chatWindow = new JoinChatWindow(this, server);
        join_chatWindow->show();
    }
}
