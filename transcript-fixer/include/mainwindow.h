#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "include/mediacontrol.h"
#include "include/transcriptionmanager.h"
#include <QMainWindow>
#include <QTableWidgetItem>
#include <QCloseEvent>

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

private:
    Ui::MainWindow *ui;
    MediaControl *mediaControl;
    TranscriptionManager *transcriptionManager;

    void initialize_ui();
    void connect_signals();
    void set_default_icons();

    void enable_media_interface();
    void disable_media_interface();
    void update_media_label(const QString &fileName);
    void update_transcription_label(const QString &fileName);

private slots:
    void on_volumeButton_clicked();
    void on_playButton_clicked();
    void on_playbackSpeedChanged(int index);
    void update_volume(int value);
    void update_position(qint64 position);
    void update_duration(qint64 duration);
    void handle_media_status_changed(QMediaPlayer::MediaStatus status);
    void jump_to_time(QTableWidgetItem *item);
    void on_backwardButton_clicked();
    void on_forwardButton_clicked();
    void add_row();
    void delete_row();
    void on_actionOpen_audio_video_file_triggered();
    void on_actionLoad_transcription_file_triggered();
    void on_actionSave_transcription_triggered();
    void on_actionShortcuts_triggered();
    void on_actionExit_triggered();
    void on_actionUsage_instructions_triggered();
    void on_actionAbout_TranscriptFixer_triggered();

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
