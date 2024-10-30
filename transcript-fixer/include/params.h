#ifndef PARAMS_H
#define PARAMS_H

#include <QtGlobal>
#include <QString>
#include <QStringList>

namespace params {
    constexpr qint64 SEEK_AMOUNT = 5000;
    inline const QString APP_NAME = QStringLiteral("TranscriptFixer");
    inline const QString APP_VERSION = QStringLiteral("v0.0.1-alpha");
    inline const std::vector<std::pair<QString, float>> speeds = { std::make_pair(QStringLiteral("0.5x"), 0.5f),
                                                            std::make_pair(QStringLiteral("1.0x"), 1.0f),
                                                            std::make_pair(QStringLiteral("1.5x"), 1.5f),
                                                            std::make_pair(QStringLiteral("2.0x"), 2.0f) };
    inline const QStringList FILE_TYPES = {QStringLiteral("Transcription"), QStringLiteral("Subtitle")};
    constexpr int DEFAULT_VOLUME = 50;
    constexpr int MAX_CHAR_PER_LINE = 80;

    inline const QString MEDIA_FILE_UNOPEN_MESSAGE = QStringLiteral("Media file not opened.");
    inline const QString TRANSCRIPTION_FILE_UNOPEN_MESSAGE = QStringLiteral("Transcription file not opened.");
}

#endif // PARAMS_H
