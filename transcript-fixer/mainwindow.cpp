#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , isPlaying(false)
    , isMuted(false)
{
    ui->setupUi(this);
    mediaPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    mediaPlayer->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.5);
    ui->volumeSlider->setRange(0, 100);
    ui->volumeSlider->setValue(50);

    connect(ui->volumeSlider, &QSlider::valueChanged, this, [this](int value) {
        audioOutput->setVolume(pow(value / 100.0, 2)); // Apply exponential scaling
    });

    ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->volumeButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    ui->backwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->forwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mediaPlayer;
    delete audioOutput;
}

void MainWindow::on_volumeButton_clicked()
{
    isMuted = !isMuted;
    if(isMuted)
    {
        ui->volumeButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
    } else {
        ui->volumeButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    }
    audioOutput->setMuted(isMuted);
}

void MainWindow::on_playButton_clicked()
{
    isPlaying = !isPlaying;
    if(isPlaying)
    {
        ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        mediaPlayer->play();
    } else {
        ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        mediaPlayer->pause();
    }
}

void MainWindow::on_actionOpen_audio_video_file_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Select Audio or Video file",
                                                    QDir::homePath(),
                                                    "Audio Files (*.mp3, *.wav);;Video Files (*.mp4, *.mkv, *.avi);;All Files (*.*)"
                                                    );
    // Check if a file was selected
    if (!filePath.isEmpty())
    {
        mediaPlayer->setSource(QUrl::fromLocalFile(filePath));

        // Extract the base name of the file
        QFileInfo fileInfo(filePath);
        ui->audioVideoFilenameLabel->setText(fileInfo.fileName());
    }
}

