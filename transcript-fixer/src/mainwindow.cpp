#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include "include/utils.h"
#include "include/params.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow),
    mediaControl(new MediaControl)

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
    this->cleanup_actions();
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

    this->set_menu_actions();

    this->update_media_label("");
    this->update_transcription_label("");

    this->disable_transcription_interface();
    this->disable_media_interface();
}

void MainWindow::connect_signals()
{
    connect(ui->playButton, &QPushButton::clicked, this, &MainWindow::play_button_clicked);
    connect(ui->volumeButton, &QPushButton::clicked, this, &MainWindow::volume_button_clicked);
    connect(ui->speedComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::change_playback_rate);
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &MainWindow::update_media_volume);
    connect(mediaControl->get_media_player(), &QMediaPlayer::positionChanged, this, &MainWindow::update_audio_slider_position);
    connect(mediaControl->get_media_player(), &QMediaPlayer::durationChanged, this, &MainWindow::update_audio_slider_duration);
    connect(ui->audioSlider, &QSlider::sliderMoved, mediaControl->get_media_player(),&QMediaPlayer::setPosition);
    connect(mediaControl->get_media_player(), &QMediaPlayer::mediaStatusChanged, this, &MainWindow::handle_media_status_changed);
    connect(ui->backwardButton, &QPushButton::clicked, this, &MainWindow::backward_button_clicked);
    connect(ui->forwardButton, &QPushButton::clicked, this, &MainWindow::forward_button_clicked);
    connect(ui->tableWidget, &QTableWidget::itemClicked, this, &MainWindow::jump_to_time);
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::add_row);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::delete_row);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::save_transcription);
    connect(ui->adjustLongLinesButton, &QPushButton::clicked, this, &MainWindow::adjust_transcription_lines);
    connect(ui->tableWidget, &QTableWidget::cellChanged, this, [this](int row, int column) {
        transcriptionManager->change_transcription_element(row, column);
    });
}

void MainWindow::set_default_icons()
{
    ui->playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->volumeButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    ui->backwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->forwardButton->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));

    ui->playButton->setToolTip("Play/Pause (Space)");
    ui->backwardButton->setToolTip("Backward (Left arrow)");
    ui->forwardButton->setToolTip("Forward (Right Arrow)");
    ui->volumeButton->setToolTip("Mute/Unmute (Ctrl + M)");
    ui->addButton->setToolTip("Add row (Ctrl + +)");
    ui->deleteButton->setToolTip("Delete row (Ctrl + -)");
    ui->saveButton->setToolTip("Save As (Ctrl + S)");
}

void MainWindow::cleanup_actions()
{
    for (QAction* action : fileActions)
    {
        delete action;
    }
    fileActions.clear();

    for (QAction* action : editActions)
    {
        delete action;
    }
    editActions.clear();

    for (QAction* action : helpActions)
    {
        delete action;
    }
    helpActions.clear();
}

void MainWindow::enable_media_interface()
{
    ui->backwardButton->setEnabled(true);
    ui->forwardButton->setEnabled(true);
    ui->playButton->setEnabled(true);
    ui->volumeButton->setEnabled(true);
    ui->volumeSlider->setEnabled(true);
    ui->speedComboBox->setEnabled(true);
}

void MainWindow::disable_media_interface()
{
    ui->backwardButton->setEnabled(false);
    ui->forwardButton->setEnabled(false);
    ui->playButton->setEnabled(false);
    ui->volumeButton->setEnabled(false);
    ui->volumeSlider->setEnabled(false);
    ui->speedComboBox->setEnabled(false);
}

void MainWindow::enable_transcription_interface()
{
    ui->addButton->setEnabled(true);
    ui->deleteButton->setEnabled(true);
    ui->adjustLongLinesButton->setEnabled(true);
    ui->saveButton->setEnabled(true);
    ui->tableWidget->setEnabled(true);
    QAction* saveAction = this->find_action_by_text(fileActions, "&Save Transcription");
    if (saveAction != nullptr)
    {
        saveAction->setEnabled(false);
    }
}

void MainWindow::disable_transcription_interface()
{
    ui->addButton->setEnabled(false);
    ui->deleteButton->setEnabled(false);
    ui->adjustLongLinesButton->setEnabled(false);
    ui->saveButton->setEnabled(false);
    ui->tableWidget->setEnabled(false);
    QAction* saveAction = this->find_action_by_text(fileActions, "&Save Transcription");
    if (saveAction != nullptr)
    {
        saveAction->setEnabled(false);
    }
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

void MainWindow::set_menu_actions()
{
    // File Actions
    fileActions.push_back(new QAction(tr("&Open Media File"), this));
    fileActions.last()->setShortcut(QKeySequence::Open);
    fileActions.push_back(new QAction(tr("&Load Transcription File"), this));
    fileActions.last()->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
    fileActions.push_back(new QAction(tr("&Save Transcription"), this));
    fileActions.last()->setShortcut(QKeySequence::Save);
    fileActions.push_back(new QAction(tr("&Quit Application"), this));
    fileActions.last()->setShortcut((QKeySequence::Quit));
    for (QAction* action : fileActions)
    {
        ui->menuFile->addAction(action);
    }

    // Help Actions
    helpActions.push_back(new QAction(tr("Shortcuts"), this));
    helpActions.push_back(new QAction(tr("Usage Instructions"), this));
    helpActions.push_back(new QAction(tr("About %1").arg(params::APP_NAME), this));
    for (QAction* action : helpActions)
    {
        ui->menuHelp->addAction(action);
    }

    // Make the connections
    this->set_menu_connections();
}

void MainWindow::set_menu_connections()
{
    // File connections
    connect(fileActions.at(0), &QAction::triggered, this, &MainWindow::open_media_file);
    connect(fileActions.at(1), &QAction::triggered, this, &MainWindow::load_transcription_file);
    connect(fileActions.at(2), &QAction::triggered, this, &MainWindow::save_transcription);
    connect(fileActions.at(3), &QAction::triggered, this, &MainWindow::quit_application);

    // Help connections
    connect(helpActions.at(0), &QAction::triggered, this, &MainWindow::show_shortcuts);
    connect(helpActions.at(1), &QAction::triggered, this, &MainWindow::show_instructions);
    connect(helpActions.at(2), &QAction::triggered, this, &MainWindow::show_about);
}

QAction* MainWindow::find_action_by_text(const QVector<QAction*>& actions, const QString text)
{
    QAction* result = nullptr;
    for (int i = 0; i < actions.size(); ++i)
    {
        if (actions.at(i)->text() == text)
        {
            result = actions.at(i);
        }
    }
    return result;
}

void MainWindow::volume_button_clicked()
{
    bool isMuted = mediaControl->is_muted();
    ui->volumeButton->setIcon(style()->standardIcon(isMuted ? QStyle::SP_MediaVolume : QStyle::SP_MediaVolumeMuted));
    mediaControl->mute(!isMuted);
}

void MainWindow::play_button_clicked()
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

void MainWindow::change_playback_rate()
{
    qreal rate = ui->speedComboBox->currentData().toReal();
    mediaControl->set_playback_rate(rate);
}

void MainWindow::update_media_volume(int value)
{
    mediaControl->set_volume(value);
}

void MainWindow::update_audio_slider_position(qint64 position)
{
    ui->audioSlider->setValue(static_cast<int>(position));
    ui->currentTimeLabel->setText(utils::format_time(position));
}

void MainWindow::update_audio_slider_duration(qint64 duration)
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

void MainWindow::backward_button_clicked()
{
    qint64 newPosition = mediaControl->get_position() - params::SEEK_AMOUNT;
    newPosition = newPosition < 0 ? 0 : newPosition;
    mediaControl->set_position(newPosition);
}

void MainWindow::forward_button_clicked()
{
    qint64 newPosition = mediaControl->get_position() + params::SEEK_AMOUNT;
    qint64 duration = mediaControl->get_duration();
    newPosition = newPosition > duration ? duration : newPosition;
    mediaControl->set_position(newPosition);
}

void MainWindow::add_row()
{
    // Block signals to prevent cell Changed from being triggered
    ui->tableWidget->blockSignals(true);
    int currentRow = ui->tableWidget->currentRow();

    if (currentRow == -1)
    {
        currentRow = ui->tableWidget->rowCount();
    }

    ui->tableWidget->insertRow(currentRow);

    ui->tableWidget->setItem(currentRow, 0, new QTableWidgetItem("00:00:00"));
    ui->tableWidget->setItem(currentRow, 1, new QTableWidgetItem("00:00:00"));
    ui->tableWidget->setItem(currentRow, 2, new QTableWidgetItem(""));

    // Re-enable signals
    ui->tableWidget->blockSignals(false);

    transcriptionManager->insert_transcription_element(currentRow);
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
        transcriptionManager->remove_transcription_element(currentRow);
    }
}

void MainWindow::adjust_transcription_lines()
{
    long long int i = 0;
    while (i < transcriptionManager->element_count())
    {
        transcriptionManager->split_transcription_element(i, params::MAX_CHAR_PER_LINE);
        ++i;
    }
    transcriptionManager->update_table();
}

void MainWindow::open_media_file()
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

void MainWindow::load_transcription_file()
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
    this->enable_transcription_interface();
}

void MainWindow::save_transcription()
{
    QString fileType = utils::select_file_type();

    QString filter = QStringLiteral("Text files (*.txt);;Subtitle files (*.srt)");
    QString filePath = QFileDialog::getSaveFileName(this,
                                                    "Save File",
                                                    QDir::homePath(), filter);

    if (fileType != "")
    {
        if(fileType == params::FILE_TYPES.at(0))    // Transcription type
        {
            transcriptionManager->save_transcription_as_txt(filePath);
        }
        else if(fileType == params::FILE_TYPES.at(1))   // Subtitle type
        {
            transcriptionManager->save_transcription_as_srt(filePath);
        }
        else
        {
            QMessageBox::warning(this, "Error", "Could not save the file.");
        }
    }
    else
    {
        QMessageBox::warning(this, "Error", "Could not save the file.");
    }
}

void MainWindow::quit_application()
{
    QApplication::quit();
}

void MainWindow::show_shortcuts()
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

void MainWindow::show_instructions()
{
    QString usageText =
        "TranscriptFixer Application - Usage Instructions:\n\n"
        "1. Open a media file (audio/video) using the 'Open Media File' menu.\n"
        "2. Load a transcription file by selecting 'Load Transcription File'.\n"
        "3. Use the media player controls to play, pause, rewind, and fast-forward.\n"
        "4. Edit the transcription text directly in the grid.\n"
        "5. Use the 'Save' option to save your changes.\n"
        "6. You can search, undo, and redo changes using the provided buttons.\n\n"
        "For more details, refer to the documentation or contact support.";

    QMessageBox::information(this, tr("Usage Instructions"), usageText);
}


void MainWindow::show_about()
{
    QString aboutText =
        params::APP_NAME + " - " + params::APP_VERSION + "\n\n"
                                                         "Developed by Ignacio Belitzky\n"
                                                         "This application allows users to correct and edit transcription text with timestamps.\n"
                                                         "Features include media playback synchronization with transcriptions, text search, undo/redo functionality, and more.\n\n"
                                                         "For feedback or issues, please contact ignabelitzky@mi.unc.edu.ar.";

    QMessageBox::information(this, tr("About TranscriptFixer"), aboutText);
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

