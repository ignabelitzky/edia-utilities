#include "utils.h"

QString utils::format_time(const qint64 ms)
{
    int hours = (ms / (1000 * 60 * 60)) % 24;
    int minutes = (ms / (1000 * 60)) % 60;
    int seconds = (ms / 1000) % 60;
    return QTime(hours, minutes, seconds).toString("HH:mm:ss");
}

qint64 utils::convert_time_to_ms(const QString &timeString)
{
    QTime time = QTime::fromString(timeString, "hh:mm:ss");
    if (!time.isValid())
    {
        return 0;
    }
    return QTime(0, 0).msecsTo(time);
}

qint64 utils::extract_start_time(const QString &text)
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

qint64 utils::extract_end_time(const QString &text)
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

Element* utils::extract_transcription_data(const QString &line)
{
    // Define the regex pattern
    QRegularExpression re(R"(\[(\d{2}:\d{2}:\d{2})\s*-\s(\d{2}:\d{2}:\d{2})\]\s*(.*))");
    QRegularExpressionMatch match = re.match(line);

    // Initialize an empty Element struct
    Element* elem = new Element;

    // Check if the line matches the pattern
    if (match.hasMatch())
    {
        // Extract start time, end time, and text from the matched groups
        elem->startTime = match.captured(1); // First captured group is start time
        elem->endTime = match.captured(2);   // Second captured group is end time
        elem->text = match.captured(3);      // Third captured group is the transcription text
    }
    return elem;
}
