#include "include/transcriptionmanager.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>

TranscriptionManager::TranscriptionManager(QObject *parent, QTableWidget *table)
    : QObject(parent), tableWidget(table)
{
}

TranscriptionManager::~TranscriptionManager()
{
    for (Element* elem : transcriptionElements)
    {
        delete elem;
    }
    transcriptionElements.clear();
}

void TranscriptionManager::load_transcription(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, "Error", "Failed to open the file.");
            return;
    }

    QTextStream in(&file);

    for (Element* elem : transcriptionElements)
    {
        delete elem;
    }
    transcriptionElements.clear();

    while (!in.atEnd())
    {
        QString line = in.readLine();
        if (!line.isEmpty())
        {
            transcriptionElements.push_back(utils::extract_transcription_data(line));
        }
    }

    file.close();
    populate_table();
}

void TranscriptionManager::save_transcription(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, "Error", "Failed to save the file.");
        return;
    }

    QTextStream out(&file);

    QString startTime = "", endTime = "", text = "";
    QTableWidgetItem *startTimeItem = nullptr, *endTimeItem = nullptr, *textItem = nullptr;
    for (int i = 0; i < tableWidget->rowCount(); ++i)
    {
        startTimeItem = tableWidget->item(i, 0);
        endTimeItem = tableWidget->item(i, 1);
        textItem = tableWidget->item(i, 2);

        startTime = startTimeItem ? startTimeItem->text() : "";
        endTime = endTimeItem ? endTimeItem->text() : "";
        text = textItem ? textItem->text() : "";

        out << QString("[%1 - %2] %3\n").arg(startTime, endTime, text);
    }

    file.close();
}

void TranscriptionManager::populate_table()
{
    tableWidget->clearContents();
    tableWidget->setRowCount(transcriptionElements.size());
    Element *elem = nullptr;
    for (int i = 0; i < transcriptionElements.size(); ++i)
    {
        elem = transcriptionElements[i];
        tableWidget->setItem(i, 0, new QTableWidgetItem(elem->startTime));
        tableWidget->setItem(i, 1, new QTableWidgetItem(elem->endTime));
        tableWidget->setItem(i, 2, new QTableWidgetItem(elem->text));
    }
    tableWidget->resizeColumnsToContents();
}

