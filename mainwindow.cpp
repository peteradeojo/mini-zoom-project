#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timer(new QTimer(this))
    , server(nullptr)
    , serverThread(nullptr)
{
    ui->setupUi(this);

    // open default camera
// #ifdef _WIN32
//     cap.open(0);
// #elif __linux__
//     cap.open("media/test_video.mp4");
// #endif
//     if (!cap.isOpened()) {
//         qDebug("Failed to open camera");
//         return;
//     }

    connect(timer, &QTimer::timeout, this, &MainWindow::captureFrame);
    timer->start(30); // 30ms interval ~33fps

    connect(ui->startServerButton, &QPushButton::clicked, this, &MainWindow::on_startServerButton_clicked);

    connect(ui->connectButton, &QPushButton::clicked, this, &MainWindow::connectButton_clicked);
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
    // cap.release();
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

void MainWindow::on_startServerButton_clicked() {
    this->hide();
    if (!serverWindow) {
        server = new MiniZoom::AppServer();

        serverWindow = new ServerWindow(server);
        serverWindow->show();
    }
}

void MainWindow::connectButton_clicked() {
    this->hide();
    if (!connectWindow) {
        connectWindow = new ConnectWindow();
        connectWindow->show();
    }
}
