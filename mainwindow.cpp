#include "mainwindow.h"
#include "VoiceReceiver.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

#include <QMediaDevices>
#include <QAudioSink>
#include <QAudioFormat>
#include <QAudioDevice>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget* central = new QWidget(this);
    setCentralWidget(central);

    recorder = new AudioRecorder(this);
    sender = new VoiceSender(QHostAddress("127.0.0.1"), 56565, this);  // replace with actual IP
    receiver = new VoiceReceiver(56565, this);

    recordButton = new QPushButton("Record", central);
    recordButton->setGeometry(30, 30, 100, 30);

    sendButton = new QPushButton("Send", central);
    sendButton->setGeometry(140, 30, 100, 30);
    sendButton->setEnabled(false);

    deleteButton = new QPushButton("Delete", central);
    deleteButton->setGeometry(250, 30, 100, 30);
    deleteButton->setEnabled(false);

    playButton = new QPushButton("Play", this);
    playButton->setGeometry(270, 100, 100, 30);  // ⬅️ Adjust position if needed
    connect(playButton, &QPushButton::clicked, this, &MainWindow::onPlayClicked);

    startCallButton = new QPushButton("Start Call", central);
    startCallButton->setGeometry(30, 80, 150, 30);

    endCallButton = new QPushButton("End Call", central);
    endCallButton->setGeometry(190, 80, 150, 30);

    connect(recordButton, &QPushButton::clicked, this, &MainWindow::toggleRecording);
    connect(sendButton, &QPushButton::clicked, this, &MainWindow::sendRecording);
    connect(deleteButton, &QPushButton::clicked, this, &MainWindow::deleteRecording);
    connect(startCallButton, &QPushButton::clicked, this, &MainWindow::startCall);
    connect(endCallButton, &QPushButton::clicked, this, &MainWindow::endCall);
}

MainWindow::~MainWindow()
{
    recorder->stopRecording();
    sender->stop();
    receiver->stop();
}

void MainWindow::toggleRecording()
{
    if (!isRecording) {
        recorder->startRecording("message.wav");
        recordButton->setText("Stop");
        sendButton->setEnabled(false);
        deleteButton->setEnabled(false);
        isRecording = true;
    } else {
        recorder->stopRecording();
        recordButton->setText("Record");
        sendButton->setEnabled(true);
        deleteButton->setEnabled(true);
        isRecording = false;
    }
}

void MainWindow::sendRecording()
{
    // Placeholder for sending the file
    QMessageBox::information(this, "Send", "Recording sent: " + recorder->getFilename());
}

void MainWindow::deleteRecording()
{
    recorder->deleteRecording();
    sendButton->setEnabled(false);
    deleteButton->setEnabled(false);
    QMessageBox::information(this, "Delete", "Recording deleted.");
}

void MainWindow::onPlayClicked()
{
    QFile* file = new QFile("message.wav", this);
    if (!file->open(QIODevice::ReadOnly)) {
        qDebug() << "Cannot open message.wav for playback.";
        return;
    }

    QAudioDevice outputDevice = QMediaDevices::defaultAudioOutput();
    QAudioFormat format;

    format.setSampleRate(44100);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    if (!outputDevice.isFormatSupported(format)) {
        qWarning() << "Standard format not supported. Using preferred format.";
        format = outputDevice.preferredFormat();
    }

    QAudioSink* sink = new QAudioSink(outputDevice, format, this);
    sink->start(file);
}

void MainWindow::startCall()
{
    // Clean up existing instances if still running
    if (receiver) {
        receiver->stop();
        delete receiver;
        receiver = nullptr;
    }

    if (sender) {
        sender->stop();
        delete sender;
        sender = nullptr;
    }

    // Start a new receiver on port 56565
    receiver = new VoiceReceiver(56565, this);
    receiver->start();
    qDebug() << "Voice call receiver started.";

    // Start the sender to send to the same port on localhost
    sender = new VoiceSender(QHostAddress::LocalHost, 56565, this);
    sender->start();
    qDebug() << "Voice call sender started.";
}


void MainWindow::endCall()
{
    if (sender) {
        sender->stop();
        delete sender;
        sender = nullptr;
        qDebug() << "Voice sending stopped and cleaned up.";
    }

    if (receiver) {
        receiver->stop();
        delete receiver;
        receiver = nullptr;
        qDebug() << "Voice receiving stopped and cleaned up.";
    }

    qDebug() << "Call ended.";
}
