#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QStringList>
#include <QTime>
#include <QtGlobal>
#include <QRegularExpression>
#include <QInputDialog>

struct TranscriptionElement {
    QString startTime;
    QString endTime;
    QString text;
};

namespace utils {
    QString format_time(const qint64 ms);
    qint64 convert_time_to_ms(const QString &timeString);
    qint64 extract_start_time(const QString &text);
    qint64 extract_end_time(const QString &text);
    TranscriptionElement* extract_transcription_data(const QString &line);
    QStringList split_text_into_lines(const QString& text, int maxLength);
    std::vector<std::pair<QString, QString>>* adjust_timestamp(const QString& startTime, const QString& endTime, int totalLines);
    QString select_file_type();
    bool check_transcription_format(const QString& filePath);
}

#endif // UTILS_H
