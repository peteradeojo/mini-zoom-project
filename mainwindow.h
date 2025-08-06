#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "AudioRecorder.h"
#include "VoiceSender.h"
#include "VoiceReceiver.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void toggleRecording();
    void sendRecording();
    void deleteRecording();
    void startCall();
    void endCall();
    void onPlayClicked();

private:
    QPushButton *recordButton, *sendButton, *deleteButton;
    QPushButton *startCallButton, *endCallButton;
    QPushButton* playButton;
    AudioRecorder* recorder;
    VoiceSender* sender = nullptr;
    VoiceReceiver* receiver = nullptr;
    bool isRecording = false;
};

#endif // MAINWINDOW_H
