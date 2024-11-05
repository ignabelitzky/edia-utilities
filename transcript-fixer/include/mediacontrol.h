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
    QMediaPlayer* get_media_player() const { return mediaPlayer; };
    void set_source(QUrl source);

    void play();
    void pause();
    bool is_playing();
    void set_position(qint64 position);
    qint64 get_position() const;
    qint64 get_duration() const;
    void set_playback_rate(qreal rate);
    void set_volume(int volume);
    void mute(bool muted);
    bool is_muted();

private:
    QMediaPlayer *mediaPlayer;
    QAudioOutput *audioOutput;
};

#endif // MEDIACONTROL_H
