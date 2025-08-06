#include "wavwriter.h"
#include <QDataStream>
#include <QDebug>

WavWriter::WavWriter(const QString& filename, const QAudioFormat& format)
    : file(filename), audioFormat(format)
{}

WavWriter::~WavWriter()
{
    finalize();
}

QIODevice* WavWriter::start()
{
    if (file.isOpen()) file.close();

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "WAV file could not be opened.";
        return nullptr;
    }

    file.write(QByteArray(44, '\0'));  // Reserve header
    return &file;
}

void WavWriter::finalize()
{
    if (!file.isOpen()) return;

    qint64 dataSize = file.size() - 44;
    file.seek(0);

    QByteArray header;
    QDataStream out(&header, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);

    // Use values from QAudioFormat
    quint16 channels = audioFormat.channelCount();
    quint32 sampleRate = audioFormat.sampleRate();
    quint16 bytesPerSample = audioFormat.bytesPerFrame() / channels;
    quint16 blockAlign = audioFormat.bytesPerFrame();
    quint32 byteRate = sampleRate * blockAlign;
    quint16 bitsPerSample = bytesPerSample * 8;

    quint16 audioFormatCode = 1; // PCM

    header.append("RIFF");
    out << quint32(dataSize + 36);
    header.append("WAVE");

    header.append("fmt ");
    out << quint32(16);
    out << audioFormatCode;
    out << channels;
    out << sampleRate;
    out << byteRate;
    out << blockAlign;
    out << bitsPerSample;

    header.append("data");
    out << quint32(dataSize);

    file.write(header);
    file.close();
}
