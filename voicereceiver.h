#ifndef VOICERECEIVER_H
#define VOICERECEIVER_H

#include <QObject>
#include <QUdpSocket>
#include <QAudioSink>
#include <QIODevice>
#include <QAudioFormat>

class VoiceReceiver : public QObject
{
    Q_OBJECT

public:
    explicit VoiceReceiver(quint16 port, QObject* parent = nullptr);
    void start();
    void stop();

private slots:
    void processPendingDatagrams();

private:
    QUdpSocket* udpSocket;
    QAudioSink* audioSink;
    QIODevice* outputDevice;
    QAudioFormat format;
};

#endif // VOICERECEIVER_H
