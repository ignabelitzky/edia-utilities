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

    // Waveform visual configuration
    setMinimumHeight(50);
    setMinimumWidth(1000);
    xAxis->setVisible(false);
    yAxis->setVisible(false);
    xAxis->grid()->setVisible(false);
    yAxis->grid()->setVisible(false);
    axisRect()->setMargins(QMargins(0, 0, 0, 0));
    setBackground(QBrush(Qt::transparent));


    // Initialize the QTimer
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, [this]() {
        // Call the updateWaveform function with the current playback time
        updateWaveform(this->mediaControl->get_position());
    });

    // Set the timer interval to 200 milliseconds
    updateTimer->start(200);

    connect(decoder, &QAudioDecoder::bufferReady, this, &Waveform::setBuffer);
}

Waveform::~Waveform()
{
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
    // Check if the decoder is in a state where reading is possible
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
        // Convert 10 seconds to samples
        int timeWindow = sampleRate * 10;

        // Find the center sample index corresponding to currentTimeMs
        int centerSampleIndex = (currentTimeMs / 1000.0) * sampleRate;

        // Calculate start and end indices for a 10-second window around the center
        int startSampleIndex = qMax(0, centerSampleIndex - timeWindow / 2);
        int endSampleIndex = qMin(samples.size(), centerSampleIndex + timeWindow / 2);

        // Extract the 10-second segment
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

        // Set the marker to the actual current playback sample index
        marker->start->setCoords(centerSampleIndex, -1);
        marker->end->setCoords(centerSampleIndex, 1);

        replot();
    }
}
