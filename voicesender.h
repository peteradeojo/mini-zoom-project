#ifndef VOICESENDER_H
#define VOICESENDER_H

#include <QObject>
#include <QUdpSocket>
#include <QAudioSource>
#include <QIODevice>
#include <QHostAddress>
#include <QAudioFormat>

class VoiceSender : public QObject
{
    Q_OBJECT

public:
    explicit VoiceSender(const QHostAddress& dest, quint16 port, QObject* parent = nullptr);
    void start();
    void stop();

private:
    QUdpSocket* udpSocket;
    QAudioSource* audioSource;
    QIODevice* inputDevice;
    QHostAddress destination;
    quint16 destPort;
    QAudioFormat format;
};

#endif // VOICESENDER_H
