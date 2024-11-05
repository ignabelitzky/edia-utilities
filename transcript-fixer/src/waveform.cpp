#include "include/waveform.h"
#include <QtGlobal>

Waveform::Waveform(MediaControl* mediaControl, QWidget *parent) : QCustomPlot(parent)
    , decoder(new QAudioDecoder(this)), marker(new QCPItemLine(this))
    , durationMs(0), sampleRate(0)
    , mediaControl(mediaControl)
{
    marker->setPen(QPen(Qt::red));
    marker->start->setCoords(0, -1);
    marker->end->setCoords(0, 1);
    wavePlot = addGraph();
    setMinimumHeight(50);

    // Initialize the QTimer
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, [this]() {
        // Call the updateWaveform function with the current playback time
        updateWaveform(this->mediaControl->get_position());
    });

    // Set the timer interval to X milliseconds
    updateTimer->start(500);

    connect(decoder, &QAudioDecoder::bufferReady, this, &Waveform::setBuffer);
    //connect(decoder, &QAudioDecoder::finished, this, &Waveform::updateWaveform);
}

Waveform::~Waveform()
{
    delete decoder;
    delete marker;
}

void Waveform::setSource(const QString& fileName)
{
    samples.clear();
    decoder->setSource(QUrl::fromLocalFile(fileName));
    decoder->start();
    durationMs = decoder->duration();
}

void Waveform::setBuffer()
{
    buffer = decoder->read();

    // Set sampleRate only once
    if (sampleRate == 0)
    {
        sampleRate = buffer.format().sampleRate();
    }

    qreal peak = getPeakValue(buffer.format());
    const qint16 *data = buffer.constData<qint16>();
    int channelCount = buffer.format().channelCount();
    int count = buffer.sampleCount() / channelCount;
    for (int i = 0; i < count; ++i)
    {
        double val = data[i] / peak;
        samples.append(val);
    }
}
/*
void Waveform::plot()
{
    int sampleCount = samples.size();
    QVector<double> xValues(sampleCount);
    for (int i = 0; i < sampleCount; ++i)
    {
        xValues[i] = i;
    }
    wavePlot->addData(xValues, samples);
    xAxis->setRange(0, sampleCount);
    yAxis->setRange(QCPRange(-1, 1));
    replot();
}
*/

qreal Waveform::getPeakValue(const QAudioFormat& format)
{
    qreal result = 0;
    QAudioFormat::SampleFormat sampleFormat = format.sampleFormat();
    switch(sampleFormat)
    {
    case QAudioFormat::UInt8:
        result = 255;
        break;
    case QAudioFormat::Int16:
        result = 32767;
        break;
    case QAudioFormat::Int32:
        result = 2147483647;
        break;
    case QAudioFormat::Float:
        result = 1.0f;
        break;
    case QAudioFormat::Unknown:
        qWarning("Unknown audio sample format");
        result = 0;
        break;
    default:
        qWarning("Error getting the peak value of the samples");
        break;
    }
    return result;
}

void Waveform::updateWaveform(qint64 currentTimeMs)
{
    if (sampleRate == 0 || samples.isEmpty())
    {
        qWarning("Sample rate is zero or no samples are loaded.");
    }
    else
    {
        // Convert 30 seconds to samples
        int samplesFor30Sec = sampleRate * 30;

        // Find the center sample index corresponding to currentTimeMs
        int centerSampleIndex = (currentTimeMs / 1000.0) * sampleRate;

        // Calculate start and end indices for a 30-second window around the center
        int startSampleIndex = qMax(0, centerSampleIndex - samplesFor30Sec / 2);
        int endSampleIndex = qMin(samples.size(), centerSampleIndex + samplesFor30Sec / 2);

        // Extract the 30-second segment
        QVector<double> xValues;
        QVector<double> segmentSamples;
        for (int i = startSampleIndex; i < endSampleIndex; ++i)
        {
            xValues.append(i);
            segmentSamples.append(samples[i]);
        }

        // Plot the extracted segment
        wavePlot->data()->clear();
        wavePlot->addData(xValues, segmentSamples);
        xAxis->setRange(xValues.first(), xValues.last());
        yAxis->setRange(QCPRange(-1, 1));
        int markerXPosition = (xValues.first() + xValues.last()) / 2;
        marker->start->setCoords(markerXPosition, -1);
        marker->end->setCoords(markerXPosition, 1);
        replot();
    }
}
