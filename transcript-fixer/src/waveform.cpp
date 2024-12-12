#include "include/waveform.h"
#include <QtGlobal>

constexpr int UPDATE_INTERVAL_MS = 600;

Waveform::Waveform(MediaControl* mediaControl, QWidget *parent) : QCustomPlot(parent)
    , decoder(new QAudioDecoder(this)), marker(new QCPItemLine(this))
    , durationMs(0), sampleRate(0)
    , mediaControl(mediaControl)
{
    configure_waveform_appearance();
    initialize_timer();
    connect(decoder, &QAudioDecoder::bufferReady, this, &Waveform::set_buffer);
}

Waveform::~Waveform()
{
    if (decoder) {
        decoder->stop();
    }
    delete decoder;
    delete updateTimer;
}

void Waveform::set_source(const QString& fileName)
{
    samples.clear();
    decoder->setSource(QUrl::fromLocalFile(fileName));
    decoder->start();
    durationMs = decoder->duration();
}

void Waveform::set_buffer()
{
    if (!decoder->bufferAvailable())
    {
        qWarning("No buffer available to decode.");
        return;
    }

    buffer = decoder->read();

    if (sampleRate == 0)
    {
        sampleRate = buffer.format().sampleRate();
    }

    qreal peak = get_peak_value(buffer.format());
    const qint16 *data = buffer.constData<qint16>();
    if (!data)
    {
        qWarning("Buffer data is null.");
        return;
    }

    int channelCount = buffer.format().channelCount();
    int count = buffer.sampleCount() / channelCount;
    for (int i = 0; i < count; ++i)
    {
        samples.append(data[i] / peak);
    }
}

void Waveform::update_waveform(qint64 currentTimeMs)
{
    if (sampleRate == 0 || samples.isEmpty())
    {
        qWarning("Sample rate is zero or no samples are loaded.");
    }
    else
    {
        constexpr int TIME_WINDOW = 10; // seconds
        int timeWindowSamples = sampleRate * TIME_WINDOW;

        int centerSampleIndex = (currentTimeMs / 1000.0) * sampleRate;
        int startSampleIndex = qMax(0, centerSampleIndex - timeWindowSamples / 2);
        int endSampleIndex = qMin(samples.size(), centerSampleIndex + timeWindowSamples / 2);

        QVector<double> xValues, segmentSamples;
        xValues.reserve(endSampleIndex - startSampleIndex);
        segmentSamples.reserve(endSampleIndex - startSampleIndex);
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

qreal Waveform::get_peak_value(const QAudioFormat& format)
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

void Waveform::initialize_timer()
{
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, [this]() {
        update_waveform(this->mediaControl->get_position());
    });
    updateTimer->start(UPDATE_INTERVAL_MS);
}

void Waveform::configure_waveform_appearance()
{
    marker->setPen(QPen(Qt::red));
    marker->start->setCoords(0, -1);
    marker->end->setCoords(0, 1);
    wavePlot = addGraph();

    setMinimumHeight(50);
    setMinimumWidth(1000);
    xAxis->setVisible(false);
    yAxis->setVisible(false);
    xAxis->grid()->setVisible(false);
    yAxis->grid()->setVisible(false);
    axisRect()->setMargins(QMargins(0, 0, 0, 0));
    setBackground(QBrush(Qt::transparent));
}
