#ifndef WAVWRITER_H
#define WAVWRITER_H

#include <QFile>
#include <QAudioFormat>

class WavWriter
{
public:
    explicit WavWriter(const QString& filePath, const QAudioFormat& format);
    ~WavWriter();

    QIODevice* start();
    void finalize();

private:
    QFile file;
    QAudioFormat audioFormat;
};

#endif // WAVWRITER_H
