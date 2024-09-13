#include "include/mediacontrol.h"
#include "include/params.h"

MediaControl::MediaControl(QObject *parent) : QObject(parent)
{
    mediaPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    mediaPlayer->setAudioOutput(audioOutput);
    audioOutput->setVolume(params::DEFAULT_VOLUME);
}

MediaControl::~MediaControl()
{
    delete mediaPlayer;
    delete audioOutput;
}

void MediaControl::set_source(QUrl source)
{
    mediaPlayer->setSource(source);
}

void MediaControl::play()
{
    mediaPlayer->play();
}

void MediaControl::pause()
{
    mediaPlayer->pause();
}

bool MediaControl::is_playing()
{
    return mediaPlayer->isPlaying();
}

void MediaControl::set_position(qint64 position)
{
    mediaPlayer->setPosition(position);
}

qint64 MediaControl::get_position()
{
    return mediaPlayer->position();
}

qint64 MediaControl::get_duration()
{
    return mediaPlayer->duration();
}

void MediaControl::set_playback_rate(qreal rate)
{
    mediaPlayer->setPlaybackRate(rate);
}

void MediaControl::set_volume(int volume)
{
    audioOutput->setVolume(pow(volume / 100.0, 2));
}

void MediaControl::mute(bool muted)
{
    audioOutput->setMuted(muted);
}

bool MediaControl::is_muted()
{
    return audioOutput->isMuted();
}

