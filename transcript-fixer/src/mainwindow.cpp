#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include "include/utils.h"
#include "include/params.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set the window to maximized mode
    this->showMaximized();

    // Set the window title
    this->setWindowTitle("TranscriptFixer");

    // Initialize media player and audio output
    disable_media_interface();
    mediaPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    mediaPlayer->setAudioOutput(audioOutput);

    // Set default volume
    audioOutput->setVolume(0.5);
    ui->volumeSlider->setRange(0, 100);
    ui->volumeSlider->setValue(50);

    // Configure the Table Widget
    ui->tableWidget->setGridStyle(Qt::NoPen);
    ui->tableWidget->setAlternatingRowColors(true);

    // Configure speedComboBox
    for (size_t i = 0; i < params::speeds.size(); ++i)
    {
        ui->speedComboBox->addItem(params::speeds.at(i).first, params::speeds.at(i).second);
    }

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

    for (Element* elem : transcriptionElements)
    {
        delete elem;
    }
    transcriptionElements.clear();
}

void MainWindow::on_volumeButton_clicked()
{
    bool isMuted = audioOutput->isMuted();
    ui->volumeButton->setIcon(style()->standardIcon(isMuted ? QStyle::SP_MediaVolume : QStyle::SP_MediaVolumeMuted));
    audioOutput->setMuted(!isMuted);
}

void MainWindow::on_playButton_clicked()
{
    if (!mediaPlayer->isPlaying())
    {
        mediaPlayer->play();
        ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
    else
    {
        mediaPlayer->pause();
        ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}

void MainWindow::on_playbackSpeedChanged(int index)
{
    Q_UNUSED(index);
    qreal speed = ui->speedComboBox->currentData().toReal();
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
    qint64 newPosition = mediaPlayer->position() - params::SEEK_AMOUNT;
    newPosition = newPosition < 0 ? 0 : newPosition;
    mediaPlayer->setPosition(newPosition);
}

void MainWindow::on_forwardButton_clicked()
{
    qint64 newPosition = mediaPlayer->position() + params::SEEK_AMOUNT;
    newPosition = newPosition > mediaPlayer->duration() ? mediaPlayer->duration() : newPosition;
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

        // Enable media interface
        enable_media_interface();
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

void MainWindow::on_actionShortcuts_triggered()
{
    QString shortcutsText;
    shortcutsText += "<b>Keyboard Shortcuts</b><br><br>";
    shortcutsText += "<b>Open media file:</b> Ctrl+O<br>";
    shortcutsText += "<b>Load transcription file:</b> Ctrl+L<br>";
    shortcutsText += "<b>Play/Pause:</b> Space<br>";
    shortcutsText += "<b>Rewind:</b> Left Arrow<br>";
    shortcutsText += "<b>Forward:</b> Right Arrow<br>";
    shortcutsText += "<b>Mute:</b> Ctrl+M<br>";
    shortcutsText += "<b>Jump to Time:</b> Double-click on transcription<br>";
    shortcutsText += "<b>Add row:</b> Ctrl++<br>";
    shortcutsText += "<b>Delete row:</b> Ctrl+-<br>";
    shortcutsText += "<b>Quit:</b> Ctrl+E";

    QMessageBox::information(this, "Shortcuts", shortcutsText);
}

void MainWindow::on_actionUsage_instructions_triggered()
{
    QString usageText =
        "TranscriptFixer Application - Usage Instructions:\n\n"
        "1. Open a media file (audio/video) using the 'Open Media File' menu.\n"
        "2. Load a transcription file by selecting 'Load Transcription File'.\n"
        "3. Use the media player controls to play, pause, rewind, and fast-forward.\n"
        "4. Edit the transcription text directly in the grid.\n"
        "5. Use the 'Save' option to save your changes.\n"
        "6. You can search, und, and redo changes using the provided buttons.\n\n"
        "For more details, refer to the documentation or contact support.";

    QMessageBox::information(this, tr("Usage Instructions"), usageText);
}


void MainWindow::on_actionAbout_TranscriptFixer_triggered()
{
    QString aboutText =
        params::APP_NAME + " - " + params::APP_VERSION + "\n\n"
        "Developed by Ignacio Belitzky\n"
        "This application allows users to correct and edit transcription text with timestamps.\n"
        "Features include media playback synchronization with transcriptions, text search, undo/redo functionality, and more.\n\n"
        "For feedback or issues, please contact ignabelitzky@mi.unc.edu.ar.";

    QMessageBox::information(this, tr("About TranscriptFixer"), aboutText);
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
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

    ui->playButton->setToolTip("Play");
    ui->backwardButton->setToolTip("Backward");
    ui->forwardButton->setToolTip("Forward");
    ui->volumeButton->setToolTip("Mute/Unmute");
    ui->addButton->setToolTip("Add row");
    ui->deleteButton->setToolTip("Remove row");
}

void MainWindow::populate_table()
{
    // Clear the transcription text if any
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    ui->tableWidget->setRowCount(transcriptionElements.size());
    for (unsigned long i = 0; i < transcriptionElements.size(); ++i)
    {
        const Element* elem = transcriptionElements[i];
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(elem->startTime));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(elem->endTime));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(elem->text));
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

    // Clear any previous transcription
    for (Element* elem : transcriptionElements)
    {
        delete elem;
    }
    transcriptionElements.clear();

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

void MainWindow::enable_media_interface()
{
    ui->backwardButton->setEnabled(true);
    ui->forwardButton->setEnabled(true);
    ui->playButton->setEnabled(true);
}

void MainWindow::disable_media_interface()
{
    ui->backwardButton->setEnabled(false);
    ui->forwardButton->setEnabled(false);
    ui->playButton->setEnabled(false);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Exit Confirmation", "Are you sure you want to close the application?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

