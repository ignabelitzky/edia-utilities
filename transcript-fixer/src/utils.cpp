#include "include/utils.h"
#include "include/params.h"

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

TranscriptionElement* utils::extract_transcription_data(const QString &line)
{
    // Define the regex pattern
    static QRegularExpression re(R"(\[(\d{2}:\d{2}:\d{2})\s*-\s(\d{2}:\d{2}:\d{2})\]\s*(.*))");
    QRegularExpressionMatch match = re.match(line);

    // Initialize an empty Element struct
    TranscriptionElement* elem = new TranscriptionElement;

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

QStringList utils::split_text_into_lines(const QString& text, int maxLength)
{
    QStringList words = text.split(' ');
    QStringList lines;
    QString currentLine;

    for (const QString& word : words)
    {
        if (currentLine.length() + word.length() + 1 <= maxLength)
        {
            if (!currentLine.isEmpty())
            {
                currentLine += ' ';
            }
            currentLine += word;
        }
        else
        {
            lines.push_back(currentLine);
            currentLine = word;
        }
    }
    if (!currentLine.isEmpty())
    {
        lines.push_back(currentLine);
    }
    return lines;
}

std::vector<std::pair<QString, QString>>* utils::adjust_timestamp(const QString& startTime, const QString& endTime, int totalLines)
{
    qint64 startMs = utils::convert_time_to_ms(startTime);
    qint64 endMs = utils::convert_time_to_ms(endTime);
    qint64 totalDuration = endMs - startMs;
    qint64 segmentDuration = totalDuration / totalLines;

    std::vector<std::pair<QString, QString>>* times = new std::vector<std::pair<QString, QString>>();

    for (int i = 0; i < totalLines; ++i)
    {
        times->push_back(std::make_pair(utils::format_time(startMs), utils::format_time(startMs + segmentDuration)));
        startMs += segmentDuration;
    }
    return times;
}

QString utils::select_file_type()
{
    bool ok;
    QString fileType = QInputDialog::getItem(nullptr, "Select File Type", "Choose the file format to save:", params::FILE_TYPES, 0, false, &ok);
    if (!ok && fileType.isEmpty())
    {
        fileType = "";
    }
    return fileType;
}
