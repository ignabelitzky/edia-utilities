#ifndef TRANSCRIPTIONMANAGER_H
#define TRANSCRIPTIONMANAGER_H

#include "include/utils.h"
#include <QObject>
#include <QTableWidget>
#include <QString>

class TranscriptionManager : public QObject
{
    Q_OBJECT

public:
    explicit TranscriptionManager(QObject *parent, QTableWidget *tableWidget);
    ~TranscriptionManager();
    void load_transcription(const QString &filePath);
    void save_transcription_as_txt(const QString &filePath);
    void save_transcription_as_srt(const QString &filePath);

private:
    QTableWidget *tableWidget;
    QVector<Element *> transcriptionElements;

    void populate_table();
};

#endif // TRANSCRIPTIONMANAGER_H
