#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"

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

    // Set default volume
    audioOutput->setVolume(0.5);
    ui->volumeSlider->setRange(0, 100);
    ui->volumeSlider->setValue(50);

    // Configure speedComboBox
    ui->speedComboBox->addItem("0.5x", 0.5);
    ui->speedComboBox->addItem("1.0x", 1.0);
    ui->speedComboBox->addItem("1.5x", 1.0);
    ui->speedComboBox->addItem("2.0x", 2.0);

    // Set default speed (1.0x)
    ui->speedComboBox->setCurrentIndex(1);

    // Connect speedComboBox to slot
    connect(ui->speedComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_playbackSpeedChanged);

    // Update the volume of the media
    connect(ui->volumeSlider, &QSlider::valueChanged, this, [this](int value) {
        audioOutput->setVolume(pow(value / 100.0, 2)); // Apply exponential scaling
    });

    // Update the slider position as the media plays
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, [this](qint64 position) {
        ui->audioSlider->setValue(static_cast<int>(position));
        ui->currentTimeLabel->setText(Utils::formatTime(position));
    });

    // Update the slider range when the media duration is known
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, [this](qint64 duration) {
        ui->audioSlider->setRange(0, static_cast<int>(duration));
        ui->totalDurationLabel->setText(Utils::formatTime(duration));
    });

    // Allow the user to seek to a position in the media by moving the slider
    connect(ui->audioSlider, &QSlider::sliderMoved, mediaPlayer, &QMediaPlayer::setPosition);

    // Connect the mediaStatusChanged signal to handle when the media ends
    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            isPlaying = false;
            mediaPlayer->setPosition(0);
        }
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

void MainWindow::on_playbackSpeedChanged(int index)
{
    double speed = ui->speedComboBox->currentData().toDouble();
    mediaPlayer->setPlaybackRate(speed);
}

void MainWindow::on_actionOpen_audio_video_file_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Select Audio or Video file",
                                                    QDir::homePath(),
                                                    "Audio Files (*.mp3 *.wav);;Video Files (*.mp4 *.mkv *.avi);;All Files (*.*)"
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

