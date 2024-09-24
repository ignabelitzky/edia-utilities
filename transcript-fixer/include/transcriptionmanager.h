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
    void change_transcription_element(int row, int column);
    void insert_transcription_element(int position);
    void remove_transcription_element(int position);
    void split_transcription_element(int row, int maxLength);
    void update_table();
    qsizetype element_count();

private:
    QTableWidget *tableWidget;
    QVector<TranscriptionElement *> transcriptionData;

    void populate_table();
};

#endif // TRANSCRIPTIONMANAGER_H
