#include "include/mediacontrol.h"
#include "include/params.h"

MediaControl::MediaControl(QObject *parent) : QObject(parent)
    , mediaPlayer(new QMediaPlayer(this))
    , audioOutput(new QAudioOutput(this))
    , playbackState(PlaybackState::Stopped)
{
    mediaPlayer->setAudioOutput(audioOutput);
    this->set_volume(params::DEFAULT_VOLUME);
}

MediaControl::~MediaControl()
{
    delete mediaPlayer;
    delete audioOutput;
}

void MediaControl::set_source(QUrl source)
{
    mediaPlayer->setSource(source);
    playbackState = PlaybackState::Stopped;
}

void MediaControl::play()
{
    if (playbackState != PlaybackState::Playing)
    {
        mediaPlayer->play();
        playbackState = PlaybackState::Playing;
    }
}

void MediaControl::pause()
{
    if (playbackState == PlaybackState::Playing)
    {
        mediaPlayer->pause();
        playbackState = PlaybackState::Paused;
    }
}

void MediaControl::toggle_play_pause()
{
    if (playbackState == PlaybackState::Playing)
    {
        pause();
    }
    else
    {
        play();
    }
}

MediaControl::PlaybackState MediaControl::get_state() const
{
    return playbackState;
}

qint64 MediaControl::get_position() const
{
    return mediaPlayer->position();
}

qint64 MediaControl::get_duration() const
{
    return mediaPlayer->duration();
}

void MediaControl::set_position(qint64 position)
{
    mediaPlayer->setPosition(position);
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

bool MediaControl::is_muted() const
{
    return audioOutput->isMuted();
}

