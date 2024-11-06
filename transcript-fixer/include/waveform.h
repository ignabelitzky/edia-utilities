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
    void set_source(const QString& fileName);

public slots:
    void set_buffer();
    void update_waveform(qint64 currentTimeMs);

private:
    qreal get_peak_value(const QAudioFormat& format);
    void initialize_timer();
    void configure_waveform_appearance();
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
