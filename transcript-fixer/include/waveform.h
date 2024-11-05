#ifndef WAVEFORM_H
#define WAVEFORM_H

#include "qcustomplot.h"
#include "mediacontrol.h"
#include <QAudioFormat>
#include <QAudioDecoder>

class Waveform : public QCustomPlot
{
    Q_OBJECT
public:
    explicit Waveform(MediaControl* mediaControl, QWidget *parent = nullptr);
    ~Waveform();
    void setSource(const QString& fileName);

public slots:
    void setBuffer();
    // void plot();
    void updateWaveform(qint64 currentTimeMs);

private:
    qreal getPeakValue(const QAudioFormat& format);
    QAudioDecoder *decoder;
    QAudioBuffer buffer;
    QVector<double> samples;
    QCPGraph *wavePlot;
    QCPItemLine *marker;
    qint64 durationMs;
    int sampleRate;
    QTimer* updateTimer;
    MediaControl* mediaControl;
signals:
};

#endif // WAVEFORM_H
