#include "voicereceiver.h"
#include <QMediaDevices>
#include <QAudioFormat>
#include <QDebug>

VoiceReceiver::VoiceReceiver(quint16 port, QObject* parent)
    : QObject(parent), udpSocket(new QUdpSocket(this)), audioSink(nullptr), outputDevice(nullptr)
{
    QAudioDevice outputDeviceInfo = QMediaDevices::defaultAudioOutput();
    QAudioFormat format = outputDeviceInfo.preferredFormat();  // ✅ Use preferred format

    qDebug() << "Receiver audio format:" << format.sampleRate()
             << "Hz," << format.channelCount() << "ch,"
             << format.sampleFormat();

    if (!outputDeviceInfo.isFormatSupported(format)) {
        qWarning() << "Audio format not supported by output device!";
        return;
    }

    audioSink = new QAudioSink(outputDeviceInfo, format, this);
    this->format = format;

    bool success = udpSocket->bind(port, QUdpSocket::ShareAddress);
    if (!success) {
        qWarning() << "Could not bind to port:" << port
                   << "Error:" << udpSocket->errorString();
        return;
    }

    connect(udpSocket, &QUdpSocket::readyRead, this, &VoiceReceiver::processPendingDatagrams);
}

void VoiceReceiver::start()
{
    if (audioSink && !outputDevice) {
        outputDevice = audioSink->start();
        qDebug() << "Voice receiving started.";
    }
}

void VoiceReceiver::stop()
{
    if (audioSink) {
        audioSink->stop();
        outputDevice = nullptr;
        qDebug() << "Voice receiving stopped.";
    }
}

void VoiceReceiver::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray buffer;
        buffer.resize(int(udpSocket->pendingDatagramSize()));
        udpSocket->readDatagram(buffer.data(), buffer.size());

        if (outputDevice) {
            outputDevice->write(buffer);
        }
    }
}
