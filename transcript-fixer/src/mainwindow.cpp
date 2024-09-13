#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include "include/utils.h"
#include "include/params.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), mediaControl(new MediaControl(this))

{
    ui->setupUi(this);
    transcriptionManager = new TranscriptionManager(this, ui->tableWidget);
    initialize_ui();
    connect_signals();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mediaControl;
    delete transcriptionManager;
}

void MainWindow::initialize_ui()
{
    this->showMaximized();
    this->setWindowTitle(params::APP_NAME);

    ui->volumeSlider->setRange(0, 100);
    ui->volumeSlider->setValue(params::DEFAULT_VOLUME);

    // Configure Table
    ui->tableWidget->setGridStyle(Qt::NoPen);
    ui->tableWidget->setAlternatingRowColors(true);

    // Configure Speed ComboBox
    for (const auto &speed : params::speeds)
    {
        ui->speedComboBox->addItem(speed.first, speed.second);
    }
    ui->speedComboBox->setCurrentIndex(1);

    // Set default icons
    set_default_icons();

    this->update_media_label("");
    this->update_transcription_label("");

    this->disable_media_interface();
}

void MainWindow::connect_signals()
{
    connect(ui->speedComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_playbackSpeedChanged);
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &MainWindow::update_volume);
    connect(mediaControl->get_media_player(), &QMediaPlayer::positionChanged, this, &MainWindow::update_position);
    connect(mediaControl->get_media_player(), &QMediaPlayer::durationChanged, this, &MainWindow::update_duration);
    connect(ui->audioSlider, &QSlider::sliderMoved, mediaControl->get_media_player(),&QMediaPlayer::setPosition);
    connect(mediaControl->get_media_player(), &QMediaPlayer::mediaStatusChanged, this, &MainWindow::handle_media_status_changed);
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

    ui->playButton->setToolTip("Play/Pause");
    ui->backwardButton->setToolTip("Backward");
    ui->forwardButton->setToolTip("Forward");
    ui->volumeButton->setToolTip("Mute/Unmute");
    ui->addButton->setToolTip("Add row");
    ui->deleteButton->setToolTip("Remove row");
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

void MainWindow::on_volumeButton_clicked()
{
    bool isMuted = mediaControl->is_muted();
    ui->volumeButton->setIcon(style()->standardIcon(isMuted ? QStyle::SP_MediaVolume : QStyle::SP_MediaVolumeMuted));
    mediaControl->mute(!isMuted);
}

void MainWindow::on_playButton_clicked()
{
    if (!mediaControl->is_playing())
    {
        mediaControl->play();
        ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    }
    else
    {
        mediaControl->pause();
        ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    }
}

void MainWindow::on_playbackSpeedChanged(int index)
{
    Q_UNUSED(index);
    qreal rate = ui->speedComboBox->currentData().toReal();
    mediaControl->set_playback_rate(rate);
}

void MainWindow::update_volume(int value)
{
    mediaControl->set_volume(value);
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
        mediaControl->set_position(0);
    }
}

void MainWindow::jump_to_time(QTableWidgetItem *item)
{
    int row = item->row();
    QString startTimeString = ui->tableWidget->item(row, 0)->text();
    qint64 timeInMs = utils::convert_time_to_ms(startTimeString);
    mediaControl->set_position(timeInMs);
}

void MainWindow::on_backwardButton_clicked()
{
    qint64 newPosition = mediaControl->get_position() - params::SEEK_AMOUNT;
    newPosition = newPosition < 0 ? 0 : newPosition;
    mediaControl->set_position(newPosition);
}

void MainWindow::on_forwardButton_clicked()
{
    qint64 newPosition = mediaControl->get_position() + params::SEEK_AMOUNT;
    qint64 duration = mediaControl->get_duration();
    newPosition = newPosition > duration ? duration : newPosition;
    mediaControl->set_position(newPosition);
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
        mediaControl->set_source(QUrl::fromLocalFile(filePath));

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
        transcriptionManager->load_transcription(filePath);
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
        transcriptionManager->save_transcription(filePath);
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

