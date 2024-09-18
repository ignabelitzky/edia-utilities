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
    for (TranscriptionElement* elem : transcriptionData)
    {
        delete elem;
    }
    transcriptionData.clear();
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

    for (TranscriptionElement* elem : transcriptionData)
    {
        delete elem;
    }
    transcriptionData.clear();

    while (!in.atEnd())
    {
        QString line = in.readLine();
        if (!line.isEmpty())
        {
            transcriptionData.push_back(utils::extract_transcription_data(line));
        }
    }

    file.close();
    populate_table();
}

void TranscriptionManager::save_transcription_as_txt(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, "Error", "Failed to save the file.");
        return;
    }

    QTextStream out(&file);
    for (const TranscriptionElement* element : transcriptionData)
    {
        out << "[" << element->startTime << " - " << element->endTime << "] " << element->text << "\n";
    }

    file.close();
}

void TranscriptionManager::save_transcription_as_srt(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(nullptr, "Error", "Failed tosave the file.");
        return;
    }

    QTextStream out(&file);
    qsizetype counter = 0;
    for (const TranscriptionElement* element : transcriptionData)
    {
        out << counter << "\n";
        out << element->startTime << " --> " << element->endTime << "\n";
        out << element->text << "\n\n";
        ++counter;
    }

    file.close();
}

void TranscriptionManager::change_transcription_element(int row, int column)
{
    // Ensure the row exists in the transcriptionData vector
    if (row >= 0 && row < transcriptionData.size())
    {
        QTableWidgetItem* item = tableWidget->item(row, column);
        if (item)
        {
            QString newValue = item->text();
            switch (column)
            {
            case 0:
                transcriptionData[row]->startTime = newValue;
                break;
            case 1:
                transcriptionData[row]->endTime = newValue;
                break;
            case 2:
                transcriptionData[row]->text = newValue;
                break;
            default:
                break;
            }
        }
    }
}

void TranscriptionManager::insert_transcription_element(int row)
{
    TranscriptionElement* newElement = new TranscriptionElement;
    newElement->startTime = QStringLiteral("00:00:00");
    newElement->endTime = QStringLiteral("00:00:00");
    newElement->text = QStringLiteral("");

    transcriptionData.insert(transcriptionData.cbegin() + row, newElement);
}

void TranscriptionManager::remove_transcription_element(int row)
{
    if (row >= 0 && row < transcriptionData.size())
    {
        transcriptionData.erase(transcriptionData.cbegin() + row);
    }
}

void TranscriptionManager::populate_table()
{
    tableWidget->clearContents();
    tableWidget->setRowCount(transcriptionData.size());
    TranscriptionElement *elem = nullptr;
    for (qsizetype i = 0; i < transcriptionData.size(); ++i)
    {
        elem = transcriptionData[i];
        tableWidget->setItem(i, 0, new QTableWidgetItem(elem->startTime));
        tableWidget->setItem(i, 1, new QTableWidgetItem(elem->endTime));
        tableWidget->setItem(i, 2, new QTableWidgetItem(elem->text));
    }
    tableWidget->resizeColumnsToContents();
}

