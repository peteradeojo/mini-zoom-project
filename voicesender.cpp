#include "voicesender.h"
#include <QMediaDevices>
#include <QAudioDevice>
#include <QDebug>

VoiceSender::VoiceSender(const QHostAddress& dest, quint16 port, QObject* parent)
    : QObject(parent), destination(dest), destPort(port),
    udpSocket(new QUdpSocket(this)), audioSource(nullptr), inputDevice(nullptr)
{
    QAudioDevice inputDeviceInfo = QMediaDevices::defaultAudioInput();
    format = inputDeviceInfo.preferredFormat();  // ✅ Use preferred input format

    qDebug() << "Sender audio format:" << format.sampleRate()
             << "Hz," << format.channelCount()
             << "ch," << format.sampleFormat();

    if (!inputDeviceInfo.isFormatSupported(format)) {
        qWarning() << "Input format not supported!";
        return;
    }

    audioSource = new QAudioSource(inputDeviceInfo, format, this);
}

void VoiceSender::start()
{
    if (!audioSource)
        return;

    inputDevice = audioSource->start();

    connect(inputDevice, &QIODevice::readyRead, this, [=]() {
        QByteArray buffer = inputDevice->readAll();
        udpSocket->writeDatagram(buffer, destination, destPort);
    });

    qDebug() << "Voice sending started to" << destination.toString() << ":" << destPort;
}

void VoiceSender::stop()
{
    if (audioSource) {
        audioSource->stop();
        inputDevice = nullptr;
        qDebug() << "Voice sending stopped.";
    }
}
