#include "utils.h"

QString utils::format_time(const qint64 ms)
{
    int hours = (ms / (1000 * 60 * 60)) % 24;
    int minutes = (ms / (1000 * 60)) % 60;
    int seconds = (ms / 1000) % 60;
    return QTime(hours, minutes, seconds).toString("HH:mm:ss");
}

qint64 utils::extract_start_time(const QString text)
{
    static const QRegularExpression re("\\[(\\d{2}):(\\d{2}):(\\d{2}) - \\d{2}:\\d{2}:\\d{2}\\]");
    QRegularExpressionMatch match = re.match(text);

    if (match.hasMatch())
    {
        // Extract hours, minutes, and seconds
        int hours = match.captured(1).toInt();
        int minutes = match.captured(2).toInt();
        int seconds = match.captured(3).toInt();

        // Convert to milliseconds
        QTime time(hours, minutes, seconds);
        return time.msecsSinceStartOfDay();
    }
    return 0;
}

qint64 utils::extract_end_time(const QString text)
{
    static const QRegularExpression re("\\[(\\d{2}):(\\d{2}):(\\d{2}) - \\d{2}:\\d{2}:\\d{2}\\]");
    QRegularExpressionMatch match = re.match(text);

    if (match.hasMatch())
    {
        // Extract hours, minutes, and seconds
        int hours = match.captured(4).toInt();
        int minutes = match.captured(5).toInt();
        int seconds = match.captured(6).toInt();

        // Convert to milliseconds
        QTime time(hours, minutes, seconds);
        return time.msecsSinceStartOfDay();
    }
    return 0;
}
