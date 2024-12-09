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
    waveform = new Waveform(this->mediaControl, this);
    initialize_ui();
    initialize_toolbar();
    connect_signals();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mediaControl;
    delete transcriptionManager;
    cleanup_actions();
}

void MainWindow::initialize_ui()
{
    showMaximized();
    setWindowTitle(params::APP_NAME);

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

    ui->statusbar->showMessage("Welcome to TranscriptFixer! Use Ctrl+O to open a media file, or Ctrl+L to open a transcription file", 10000);

    ui->verticalLayout_4->addWidget(waveform);

    set_default_icons();

    set_menu_actions();

    update_label(ui->mediaFilenameLabel, "", params::MEDIA_FILE_UNOPEN_MESSAGE);
    update_label(ui->transcriptionFilenameLabel, "", params::TRANSCRIPTION_FILE_UNOPEN_MESSAGE);

    set_transcription_interface_enabled(false);
    set_media_interface_enabled(false);
}

void MainWindow::initialize_toolbar()
{
    ui->toolbar->addAction(fileActions.at(0));
    ui->toolbar->addAction(fileActions.at(1));
    ui->toolbar->addAction(fileActions.at(2));
    ui->toolbar->addSeparator();
    ui->toolbar->addAction(editActions.at(0));
    ui->toolbar->addAction(editActions.at(1));
    ui->toolbar->setIconSize(QSize(16, 16));
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
    ui->playButton->setIcon(QIcon(":/icons/multimedia/play"));
    ui->volumeButton->setIcon(QIcon(":/icons/multimedia/volume"));
    ui->backwardButton->setIcon(QIcon(":/icons/multimedia/backward"));
    ui->forwardButton->setIcon(QIcon(":/icons/multimedia/forward"));

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

void MainWindow::set_media_interface_enabled(bool enabled)
{
    ui->backwardButton->setEnabled(enabled);
    ui->forwardButton->setEnabled(enabled);
    ui->playButton->setEnabled(enabled);
    ui->volumeButton->setEnabled(enabled);
    ui->volumeSlider->setEnabled(enabled);
    ui->speedComboBox->setEnabled(enabled);
}

void MainWindow::set_transcription_interface_enabled(bool enabled)
{
    ui->addButton->setEnabled(enabled);
    ui->deleteButton->setEnabled(enabled);
    ui->adjustLongLinesButton->setEnabled(enabled);
    ui->saveButton->setEnabled(enabled);
    ui->tableWidget->setEnabled(enabled);
    QAction* saveAction = find_action_by_text(fileActions, "&Save Transcription");
    if (saveAction != nullptr)
    {
        saveAction->setEnabled(enabled);
    }
    for (QAction* action : editActions)
    {
        action->setEnabled(enabled);
    }
}

void MainWindow::set_menu_actions()
{
    // File Actions
    fileActions.push_back(new QAction(QIcon(":/icons/file_operations/open-media"), tr("&Open Media File"), this));
    fileActions.last()->setShortcut(QKeySequence::Open);
    fileActions.push_back(new QAction(QIcon(":/icons/file_operations/open-transcription"), tr("&Load Transcription File"), this));
    fileActions.last()->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
    fileActions.push_back(new QAction(QIcon(":/icons/file_operations/save-as"), tr("&Save As"), this));
    fileActions.last()->setShortcut(QKeySequence::Save);
    fileActions.push_back(new QAction(QIcon(":/icons/file_operations/close"), tr("&Quit Application"), this));
    fileActions.last()->setShortcut((QKeySequence::Quit));
    for (QAction* action : fileActions)
    {
        ui->menuFile->addAction(action);
    }

    // Edit Actions
    editActions.push_back(new QAction(QIcon(":/icons/row_management/add"), tr("Add row"), this));
    editActions.push_back(new QAction(QIcon(":/icons/row_management/remove"), tr("Delete row"), this));
    for (QAction* action : editActions)
    {
        ui->menuEdit->addAction(action);
    }

    // Help Actions
    helpActions.push_back(new QAction(tr("Shortcuts"), this));
    helpActions.push_back(new QAction(tr("Usage Instructions"), this));
    helpActions.push_back(new QAction(QIcon(":/icons/information/about"), tr("About %1").arg(params::APP_NAME), this));
    for (QAction* action : helpActions)
    {
        ui->menuHelp->addAction(action);
    }

    // Make the connections
    set_menu_connections();
}

void MainWindow::set_menu_connections()
{
    // File connections
    connect(fileActions.at(0), &QAction::triggered, this, &MainWindow::open_media_file);
    connect(fileActions.at(1), &QAction::triggered, this, &MainWindow::load_transcription_file);
    connect(fileActions.at(2), &QAction::triggered, this, &MainWindow::save_transcription);
    connect(fileActions.at(3), &QAction::triggered, this, &MainWindow::quit_application);

    // Edit connections
    connect(editActions.at(0), &QAction::triggered, this, &MainWindow::add_row);
    connect(editActions.at(1), &QAction::triggered, this, &MainWindow::delete_row);

    // Help connections
    connect(helpActions.at(0), &QAction::triggered, this, &MainWindow::show_shortcuts);
    connect(helpActions.at(1), &QAction::triggered, this, &MainWindow::show_instructions);
    connect(helpActions.at(2), &QAction::triggered, this, &MainWindow::show_about);
}

void MainWindow::update_label(QLabel* label, const QString& fileName, const QString& defaultText)
{
    if (fileName.isEmpty())
    {
        label->setText(defaultText);
        label->setStyleSheet("color: red;");
    }
    else
    {
        label->setText(fileName);
        label->setStyleSheet("");
    }
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
    ui->volumeButton->setIcon(QIcon(isMuted ? ":/icons/multimedia/volume" : ":/icons/multimedia/mute"));
    mediaControl->mute(!isMuted);
}

void MainWindow::play_button_clicked()
{
    if (mediaControl->get_state() == MediaControl::PlaybackState::Playing)
    {
        mediaControl->pause();
        ui->playButton->setIcon(QIcon(":/icons/multimedia/play"));
    }
    else
    {
        mediaControl->play();
        ui->playButton->setIcon(QIcon(":/icons/multimedia/pause"));
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
        ui->playButton->setIcon(QIcon(":/icons/multimedia/play"));
        mediaControl->set_position(0);
        mediaControl->pause();
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
    const QString defaultTimestamp = "00:00:00,000";
    const QString defaultText = "";

    // Block signals ssafely with QSignalBlocker
    QSignalBlocker blocker(ui->tableWidget);

    if (!ui->tableWidget)
        return;

    int currentRow = ui->tableWidget->currentRow();
    if (currentRow == -1)
    {
        currentRow = ui->tableWidget->rowCount();
    }

    // Insert a new row at the specified position
    ui->tableWidget->insertRow(currentRow);

    // Create items with default values and set alignment
    QTableWidgetItem *startItem = new QTableWidgetItem(defaultTimestamp);
    QTableWidgetItem *endItem = new QTableWidgetItem(defaultTimestamp);
    QTableWidgetItem *textItem = new QTableWidgetItem(defaultText);
    ui->tableWidget->setItem(currentRow, 0, startItem);
    ui->tableWidget->setItem(currentRow, 1, endItem);
    ui->tableWidget->setItem(currentRow, 2, textItem);

    // Update the transcription manager
    transcriptionManager->insert_transcription_element(currentRow);
    ui->statusbar->showMessage("Row created", 2000);
}

void MainWindow::delete_row()
{
    if (!ui->tableWidget || ui->tableWidget->rowCount() == 0)
        return;

    // Block signals to prevent unwanted changes
    QSignalBlocker blocker(ui->tableWidget);

    int currentRow = ui->tableWidget->currentRow();
    if (currentRow == -1)
    {
        currentRow = ui->tableWidget->rowCount() - 1;
    }
    if (currentRow >= 0)
    {
        ui->tableWidget->removeRow(currentRow);
        transcriptionManager->remove_transcription_element(currentRow);
        ui->statusbar->showMessage("Row deleted", 2000);
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
    ui->statusbar->showMessage("Transcription lines adjusted", 2000);
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
        waveform->set_source(filePath);
        mediaControl->set_source(QUrl::fromLocalFile(filePath));
        ui->playButton->setIcon(QIcon(":/icons/multimedia/play"));
        ui->statusbar->showMessage("Media file loaded", 5000);

        // Extract the base name of the file
        QFileInfo fileInfo(filePath);
        update_label(ui->mediaFilenameLabel, fileInfo.fileName(), "");

        // Enable media interface
        set_media_interface_enabled(true);
    }
    else
    {
        ui->statusbar->showMessage("Warning: No media file selected", 5000);
    }
}

void MainWindow::load_transcription_file()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    "Open Transcription File",
                                                    QDir::homePath(),
                                                    "Text Files (*.txt);;All Files (*.*)");
    if (!filePath.isEmpty())
    {
        if (utils::check_transcription_format(filePath))
        {
            transcriptionManager->load_transcription(filePath);
            QFileInfo fileInfo(filePath);
            update_label(ui->transcriptionFilenameLabel, fileInfo.fileName(), "");
            set_transcription_interface_enabled(true);
            ui->statusbar->showMessage("Transcription file loaded", 5000);
        }
        else
        {
            ui->statusbar->showMessage("Warning: Unsupported file format", 5000);
        }
    }
    else
    {
        ui->statusbar->showMessage("Warning: No transcription file selected", 5000);
    }
}

void MainWindow::save_transcription()
{
    QString fileType = utils::select_file_type();

    if (!fileType.isEmpty())
    {
        QString filter = QStringLiteral("All (*.*);;Text files (*.txt);;Subtitle files (*.srt)");
        QString filePath = QFileDialog::getSaveFileName(this,
                                                        "Save File",
                                                        QDir::homePath(), filter);
        if (filePath.isEmpty())
        {
            ui->statusbar->showMessage("Warning: Save operation was cancelled", 5000);
        }
        else
        {
            if(fileType == params::FILE_TYPES.at(0))    // Transcription type
            {
                transcriptionManager->save_transcription_as_txt(filePath);
                ui->statusbar->showMessage("File saved as text transcription", 5000);
            }
            else if(fileType == params::FILE_TYPES.at(1))   // Subtitle type
            {
                transcriptionManager->save_transcription_as_srt(filePath);
                ui->statusbar->showMessage("File saved as subtitle", 5000);
            }
            else
            {
                ui->statusbar->showMessage("Error: Invalid file type selected", 5000);
            }
        }
    }
    else
    {
        ui->statusbar->showMessage("Warning: File type selection was cancelled", 5000);
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
    shortcutsText += "<b>Quit:</b> Ctrl+Q";

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

