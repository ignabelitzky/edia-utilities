#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"
#include "params.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), isPlaying(false), isMuted(false), seekAmount(appconfig::SEEK_AMOUNT)
{
    ui->setupUi(this);

    // Set the window to maximized mode
    this->showMaximized();

    // Set the window title
    this->setWindowTitle("TranscriptFixer");

    // Initialize media player and audio output
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
    ui->speedComboBox->addItem("1.5x", 1.5);
    ui->speedComboBox->addItem("2.0x", 2.0);

    // Set default speed (1.0x)
    ui->speedComboBox->setCurrentIndex(1);

    // Initialize media label and transcription label
    update_media_label("");
    update_transcription_label("");

    // Connect signals to slots
    connect_signals();

    // Set default icons
    set_default_icons();
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
    ui->volumeButton->setIcon(style()->standardIcon(isMuted ? QStyle::SP_MediaVolumeMuted : QStyle::SP_MediaVolume));
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

void MainWindow::on_playbackSpeedChanged(int index)
{
    Q_UNUSED(index);
    double speed = ui->speedComboBox->currentData().toDouble();
    mediaPlayer->setPlaybackRate(speed);
}

void MainWindow::update_volume(int value)
{
    audioOutput->setVolume(pow(value / 100.0, 2));  // Apply exponential scaling
}

void MainWindow::update_position(qint64 position)
{
    ui->audioSlider->setValue(static_cast<int>(position));
    ui->currentTimeLabel->setText(utils::format_time(position));
}

void MainWindow::update_duration(qint64 duration)
{
    ui->audioSlider->setRange(0, static_cast<int>(duration));
    ui->totalDurationLabel->setText(utils::format_time(duration));
}

void MainWindow::handle_media_status_changed(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia)
    {
        ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        isPlaying = false;
        mediaPlayer->setPosition(0);
    }
}

void MainWindow::jump_to_time(QTableWidgetItem *item)
{
    int row = item->row();
    QString startTimeString = ui->tableWidget->item(row, 0)->text();
    qint64 timeInMs = utils::convert_time_to_ms(startTimeString);
    mediaPlayer->setPosition(timeInMs);
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

void MainWindow::update_media_label(const QString &fileName)
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

void MainWindow::update_transcription_label(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        ui->transcriptionFilenameLabel->setText("Transcription file not opened.");
        ui->transcriptionFilenameLabel->setStyleSheet("color: red;");
    }
    else
    {
        ui->transcriptionFilenameLabel->setText(fileName);
        ui->transcriptionFilenameLabel->setStyleSheet("");
    }
}

void MainWindow::add_row()
{
    int currentRow = ui->tableWidget->currentRow();

    if (currentRow == -1)
    {
        currentRow = ui->tableWidget->rowCount();
    }

    ui->tableWidget->insertRow(currentRow);

    QTableWidgetItem *startTimeItem = new QTableWidgetItem("00:00:00");
    QTableWidgetItem *endTimeItem = new QTableWidgetItem("00:00:00");
    QTableWidgetItem *textItem = new QTableWidgetItem("");

    ui->tableWidget->setItem(currentRow, 0, startTimeItem);
    ui->tableWidget->setItem(currentRow, 1, endTimeItem);
    ui->tableWidget->setItem(currentRow, 2, textItem);
}

void MainWindow::delete_row()
{
    int currentRow = ui->tableWidget->currentRow();

    if (currentRow == -1)
    {
        currentRow = ui->tableWidget->rowCount() - 1;
    }

    if (ui->tableWidget->rowCount() > 0 && currentRow >= 0 && currentRow < ui->tableWidget->rowCount())
    {
        ui->tableWidget->removeRow(currentRow);
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
        update_media_label(fileInfo.fileName());
    }
    else
    {
        update_media_label("");
    }
}

void MainWindow::on_actionLoad_transcription_file_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Open Transcription File",
                                                    QDir::homePath(),
                                                    "Text Files (*.txt);;All Files (*.*)");
    // Check if a file was selected
    if (!filePath.isEmpty())
    {
        load_transcription_file(filePath);
        QFileInfo fileInfo(filePath);
        update_transcription_label(fileInfo.fileName());
    }
    else
    {
        update_transcription_label("");
    }
}

void MainWindow::on_actionSave_transcription_triggered()
{
    QString filePath = QFileDialog::getSaveFileName(this,
                                                    "Save Transcription File",
                                                    QDir::homePath(),
                                                    "Text Files (*.txt);;All Files (*.*");
    // Check if a file was selected
    if (!filePath.isEmpty())
    {
        save_transcription_file(filePath);
    }
}

void MainWindow::connect_signals()
{
    connect(ui->speedComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_playbackSpeedChanged);
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &MainWindow::update_volume);
    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &MainWindow::update_position);
    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, &MainWindow::update_duration);
    connect(ui->audioSlider, &QSlider::sliderMoved, mediaPlayer,&QMediaPlayer::setPosition);
    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::handle_media_status_changed);
    connect(ui->backwardButton, &QPushButton::clicked, this, &MainWindow::on_backwardButton_clicked);
    connect(ui->forwardButton, &QPushButton::clicked, this, &MainWindow::on_forwardButton_clicked);
    connect(ui->tableWidget, &QTableWidget::itemClicked, this, &MainWindow::jump_to_time);
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::add_row);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::delete_row);
}

void MainWindow::set_default_icons()
{
    ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->volumeButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    ui->backwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->forwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
}

void MainWindow::populate_table()
{
    ui->tableWidget->setRowCount(transcriptionElements.size());
    for (unsigned long i = 0; i < transcriptionElements.size(); ++i)
    {
        const Element& elem = transcriptionElements[i];
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(elem.startTime));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(elem.endTime));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(elem.text));
    }
}

void MainWindow::load_transcription_file(const QString &filePath)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "Failed to open the file.");
        return;
    }

    QTextStream in(&file);
    transcriptionElements.clear();  // Clear any previous transcription

    while (!in.atEnd())
    {
        QString line = in.readLine();
        if (!line.isEmpty()) {
            transcriptionElements.push_back(utils::extract_transcription_data(line));
        }
    }
    file.close();
    populate_table();
    ui->tableWidget->resizeColumnsToContents();
}

void MainWindow::save_transcription_file(const QString &filePath)
{
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Error", "Failed to save the file.");
        return;
    }

    QTextStream out(&file);

    // Iterate through all rows
    int rowCount = ui->tableWidget->rowCount();
    for (int i = 0; i < rowCount; ++i)
    {
        // Get the values from each column in the row
        QTableWidgetItem *startTimeItem = ui->tableWidget->item(i, 0);
        QTableWidgetItem *endTimeItem = ui->tableWidget->item(i, 1);
        QTableWidgetItem *textItem = ui->tableWidget->item(i, 2);

        QString startTime = startTimeItem ? startTimeItem->text() : "";
        QString endTime = endTimeItem ? endTimeItem->text() : "";
        QString text = textItem ? textItem->text() : "";

        // Format the line in the desired format
        QString line = QString("[%1 - %2] %3").arg(startTime, endTime, text);

        // Write the line to the file
        out << line << "\n";
    }

    file.close();
}

