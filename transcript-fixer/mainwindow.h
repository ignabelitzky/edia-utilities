#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtMultimedia>
#include <QStyle>
#include <QFileDialog>

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
    void on_actionOpen_audio_video_file_triggered();

private:
    Ui::MainWindow *ui;
    bool isPlaying;
    bool isMuted;
    QMediaPlayer *mediaPlayer;
    QAudioOutput *audioOutput;
};
#endif // MAINWINDOW_H
