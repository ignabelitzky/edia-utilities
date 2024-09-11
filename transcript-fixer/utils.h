#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QTime>
#include <QtGlobal>
#include <QRegularExpression>

struct Element {
    QString startTime;
    QString endTime;
    QString text;
};

namespace utils {
    QString format_time(const qint64 ms);
    qint64 convert_time_to_ms(const QString &timeString);
    qint64 extract_start_time(const QString &text);
    qint64 extract_end_time(const QString &text);
    Element* extract_transcription_data(const QString &line);

}

#endif // UTILS_H
