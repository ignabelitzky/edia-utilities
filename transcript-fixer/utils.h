#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QTime>
#include <QtGlobal>
#include <QRegularExpression>

namespace utils {
    QString format_time(const qint64 ms);
    qint64 extract_start_time(const QString text);
    qint64 extract_end_time(const QString text);
}

#endif // UTILS_H
