#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "include/mediacontrol.h"
#include "include/transcriptionmanager.h"
#include <QMainWindow>
#include <QTableWidgetItem>
#include <QCloseEvent>
#include <QThread>

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
    QThread *mediaThread;
    MediaControl *mediaControl;
    QVector<QAction*> fileActions;
    QVector<QAction*> editActions;
    QVector<QAction*> helpActions;
    TranscriptionManager *transcriptionManager;

    void initialize_ui();
    void connect_signals();
    void set_default_icons();
    void cleanup_actions();

    void enable_media_interface();
    void disable_media_interface();
    void update_media_label(const QString &fileName);
    void update_transcription_label(const QString &fileName);
    void set_menu_actions();
    void set_menu_connections();

private slots:
    void volume_button_clicked();
    void play_button_clicked();
    void change_playback_rate();
    void update_media_volume(int value);
    void update_audio_slider_position(qint64 position);
    void update_audio_slider_duration(qint64 duration);
    void handle_media_status_changed(QMediaPlayer::MediaStatus status);
    void jump_to_time(QTableWidgetItem *item);
    void backward_button_clicked();
    void forward_button_clicked();
    void add_row();
    void delete_row();
    void adjust_transcription_lines();

    // Actions slots
    void open_media_file();
    void load_transcription_file();
    void save_transcription();
    void quit_application();
    void show_shortcuts();
    void show_instructions();
    void show_about();

protected:
    void closeEvent(QCloseEvent *event) override;
};
#endif // MAINWINDOW_H
