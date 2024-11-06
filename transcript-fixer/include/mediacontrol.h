#ifndef MEDIACONTROL_H
#define MEDIACONTROL_H

#include <QMediaPlayer>
#include <QAudioOutput>

class MediaControl : public QObject
{
    Q_OBJECT

public:
    explicit MediaControl(QObject *parent = nullptr);
    ~MediaControl();

    enum class PlaybackState {
        Stopped,
        Playing,
        Paused
    };

    QMediaPlayer* get_media_player() const { return mediaPlayer; };

    // Media player controls
    void set_source(QUrl source);
    void play();
    void pause();
    void toggle_play_pause();
    PlaybackState get_state() const;

    // Position, rate, volume
    qint64 get_position() const;
    qint64 get_duration() const;
    void set_position(qint64 position);
    void set_playback_rate(qreal rate);
    void set_volume(int volume);
    void mute(bool muted);
    bool is_muted() const;

private:
    QMediaPlayer *mediaPlayer;
    QAudioOutput *audioOutput;
    PlaybackState playbackState;
};

#endif // MEDIACONTROL_H
