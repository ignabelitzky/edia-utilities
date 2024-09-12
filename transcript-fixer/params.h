#ifndef PARAMS_H
#define PARAMS_H

#include <QtGlobal>
#include <QString>

namespace params {
    constexpr qint64 SEEK_AMOUNT = 5000;
    const QString APP_NAME = QStringLiteral("TranscriptFixer");
    const QString APP_VERSION = QStringLiteral("v0.0.1-alpha");
    const std::vector<std::pair<QString, float>> speeds = { std::make_pair(QString("0.5x"), 0.5f),
                                                            std::make_pair(QString("1.0x"), 1.0f),
                                                            std::make_pair(QString("1.5x"), 1.5f),
                                                            std::make_pair(QString("2.0x"), 2.0f) };
}

#endif // PARAMS_H
