#include "audiorecorder.h"
#include "WavWriter.h"
#include <QMediaDevices>
#include <QDebug>
#include <QFile>

AudioRecorder::AudioRecorder(QObject* parent)
    : QObject(parent),
    audioSource(nullptr),
    wavWriter(nullptr)
{}

void AudioRecorder::startRecording(const QString& filename)
{
    QAudioDevice inputDevice = QMediaDevices::defaultAudioInput();
    QAudioFormat format = inputDevice.preferredFormat();  // ✅ Use preferred format

    qDebug() << "Using preferred format for recording:"
             << format.sampleRate() << "Hz,"
             << format.channelCount() << "ch,"
             << format.sampleFormat();

    QFile::remove(filename);  // Remove old file

    wavWriter = new WavWriter(filename, format);
    QIODevice* output = wavWriter->start();
    if (!output) {
        qWarning() << "Failed to open output WAV file.";
        delete wavWriter;
        wavWriter = nullptr;
        return;
    }

    audioSource = new QAudioSource(inputDevice, format, this);
    audioSource->start(output);
    currentFile = filename;
}

void AudioRecorder::stopRecording()
{
    if (audioSource) {
        audioSource->stop();
        audioSource->deleteLater();
        audioSource = nullptr;
    }

    if (wavWriter) {
        wavWriter->finalize();
        delete wavWriter;
        wavWriter = nullptr;
    }
}

void AudioRecorder::deleteRecording()
{
    if (!currentFile.isEmpty()) {
        QFile::remove(currentFile);
        currentFile.clear();
    }
}

QString AudioRecorder::getFilename() const
{
    return currentFile;
}
