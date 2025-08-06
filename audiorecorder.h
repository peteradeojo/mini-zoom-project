#ifndef AUDIORECORDER_H
#define AUDIORECORDER_H

#include <QObject>
#include <QAudioSource>
#include <QFile>
#include "WavWriter.h"

class AudioRecorder : public QObject
{
    Q_OBJECT

public:
    AudioRecorder(QObject* parent = nullptr);
    void startRecording(const QString& filename);
    void stopRecording();
    void deleteRecording();
    QString getFilename() const;

private:
    QAudioSource* audioSource;
    WavWriter* wavWriter;
    QString currentFile;
};

#endif // AUDIORECORDER_H
