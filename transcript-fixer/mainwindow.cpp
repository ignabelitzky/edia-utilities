#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"
#include "params.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), isPlaying(false)
    , isMuted(false), seekAmount(appconfig::SEEK_AMOUNT)
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

    // Initialize media label
    update_mediaLabel("");

    // Connect speedComboBox to slot
    connect(ui->speedComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_playbackSpeedChanged);

    // Update the volume of the media
    connect(ui->volumeSlider, &QSlider::valueChanged, this, [this](int value) {
        audioOutput->setVolume(pow(value / 100.0, 2)); // Apply exponential scaling
    });

    // Update the slider position as the media plays
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, [this](qint64 position) {
        ui->audioSlider->setValue(static_cast<int>(position));
        ui->currentTimeLabel->setText(utils::format_time(position));
    });

    // Update the slider range when the media duration is known
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, [this](qint64 duration) {
        ui->audioSlider->setRange(0, static_cast<int>(duration));
        ui->totalDurationLabel->setText(utils::format_time(duration));
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

    // Connect playback control buttons
    connect(ui->backwardButton, &QPushButton::clicked, this, &MainWindow::on_backwardButton_clicked);
    connect(ui->forwardButton, &QPushButton::clicked, this, &MainWindow::on_forwardButton_clicked);

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
    }
    else
    {
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
    }
    else
    {
        ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        mediaPlayer->pause();
    }
}

// Handle playback speed change
void MainWindow::on_playbackSpeedChanged(int index)
{
    Q_UNUSED(index);
    double speed = ui->speedComboBox->currentData().toDouble();
    mediaPlayer->setPlaybackRate(speed);
}

void MainWindow::on_backwardButton_clicked()
{
    qint64 currentPosition = mediaPlayer->position();
    qint64 newPosition = currentPosition - seekAmount;
    if (newPosition < 0)
    {
        newPosition = 0;
    }
    mediaPlayer->setPosition(newPosition);
}

void MainWindow::on_forwardButton_clicked()
{
    qint64 currentPosition = mediaPlayer->position();
    qint64 newPosition = currentPosition + seekAmount;
    if (newPosition > mediaPlayer->duration())
    {
        newPosition = mediaPlayer->duration();
    }
    mediaPlayer->setPosition(newPosition);
}

void MainWindow::update_mediaLabel(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        ui->mediaFilenameLabel->setText("Media file not opened.");
        ui->mediaFilenameLabel->setStyleSheet("color: red;");
    }
    else
    {
        ui->mediaFilenameLabel->setText(fileName);
        ui->mediaFilenameLabel->setStyleSheet("");
    }
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
        update_mediaLabel(fileInfo.fileName());
    }
    else
    {
        update_mediaLabel("");
    }
}

