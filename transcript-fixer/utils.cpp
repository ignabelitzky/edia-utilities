#include "utils.h"

QString utils::format_time(const qint64 ms)
{
    int hours = (ms / (1000 * 60 * 60)) % 24;
    int minutes = (ms / (1000 * 60)) % 60;
    int seconds = (ms / 1000) % 60;
    return QTime(hours, minutes, seconds).toString("HH:mm:ss");
}
