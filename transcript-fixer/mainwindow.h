#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia>
#include <QStyle>
#include <QFileDialog>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_volumeButton_clicked();
    void on_playButton_clicked();
    void on_playbackSpeedChanged(int index);
    void update_volume(int value);
    void update_position(qint64 position);
    void update_duration(qint64 duration);
    void handle_media_status_changed(QMediaPlayer::MediaStatus status);
    void jump_to_time(QListWidgetItem *item);
    void on_backwardButton_clicked();
    void on_forwardButton_clicked();
    void update_media_label(const QString &fileName);
    void update_transcription_label(const QString &fileName);
    void update_transcriptionListWidget(const QStringList &transcriptions);
    void on_actionOpen_audio_video_file_triggered();
    void on_actionLoad_transcription_file_triggered();

private:
    Ui::MainWindow *ui;
    bool isPlaying;
    bool isMuted;
    qint64 seekAmount;
    QMediaPlayer *mediaPlayer;
    QAudioOutput *audioOutput;
    void connect_signals();
    void set_default_icons();
    void load_transcription_file(const QString &filePath);
};
#endif // MAINWINDOW_H
