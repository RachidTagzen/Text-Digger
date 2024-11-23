/*
    Author: Rachid Tagzen
    Date: 2024/11/23 | 22h57 | 13h34

    This work is licensed under the MIT License.

    Copyright (c) 2024 Rachid Tagzen

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/


#pragma once

#include "models/results_model.h"

#include <QDateTime>


class StatisticsModel : public QStandardItemModel {
    Q_OBJECT


public:
    /**
     * Constructor: Initializes the model with two columns: "Statistic" and "Value".
     */
    StatisticsModel(QObject *parent, bool enableTooltips = false) : QStandardItemModel(parent), showTooltips(enableTooltips) {
        setColumnCount(2);
        setHorizontalHeaderItem(0, new QStandardItem("Statistic"));
        setHorizontalHeaderItem(1, new QStandardItem("Value"));
    }


    /**
     * Appends a new row to the statistics model with the specified statistic name and value.
     * Sets the statistic name to be bold and highlights the background.
     * @param statistic - The name of the statistic to display.
     * @param value - The corresponding value of the statistic.
     */
    void appendNew(const QString &statistic, const QString &value) {

        QFont boldFont;
        boldFont.setBold(true);

        QStandardItem *statisticItem = new QStandardItem(statistic);
        statisticItem->setData(QBrush(QColor(255, 254, 224)), Qt::BackgroundRole);
        statisticItem->setData(boldFont, Qt::FontRole);

        QStandardItem *valueItem = new QStandardItem(value);

        appendRow({statisticItem, valueItem});
    }


    /**
     * Calculates statistics from a `ResultsModel`, such as file counts, size ranges, and date ranges.
     * Clears the model before appending new statistics.
     * @param resultsModel - The model containing file data.
     * @param m_statsStartTime - Start time of data collection.
     * @param m_statsEndTime - End time of data collection.
     * @param m_statsElapsed - Elapsed time of data processing.
     * @param m_statisticsMap - A map with additional pre-calculated statistics.
     */
    void calculate(ResultsModel *&resultsModel, const QString &m_statsStartTime, const QString &m_statsEndTime,
                   const QString &m_statsElapsed, const QMap<QString, qint64> &m_statisticsMap) {

        // --------------------------
        // Clear the model before adding new statistics
        // --------------------------
        removeRows(0, rowCount());

        // --------------------------
        // Check if the `resultsModel` exists and has rows
        // --------------------------
        if (!resultsModel || resultsModel->isEmpty())
            return;


        // --------------------------
        // Initialize variables for calculating min/max statistics
        // --------------------------
        qint64 smallestFileSize = 0;
        qint64 biggestFileSize = 0;
        qint64 foundFilesSize = 0;
        int biggestOccurrences = 0;
        int totalOccurrences = 0;
        QDateTime oldestModified = QDateTime::currentDateTime();
        QDateTime newestModified = QDateTime::currentDateTime();
        QDateTime oldestCreated = QDateTime::currentDateTime();
        QDateTime newestCreated = QDateTime::currentDateTime();
        QDateTime oldestAccessed = QDateTime::currentDateTime();
        QDateTime newestAccessed = QDateTime::currentDateTime();


        // --------------------------
        // Helper function to update min/max QDateTime
        // --------------------------
        auto updateDateRange = [](const QDateTime& time, QDateTime& minTime, QDateTime& maxTime) {
            if (time.isValid()) {
                minTime = std::min(minTime, time);
                maxTime = std::max(maxTime, time);
            }
        };


        // --------------------------
        // Iterate over rows in `resultsModel` to collect statistics
        // --------------------------
        const int rowsCount = resultsModel->rowCount();

        for (int row = 0; row < rowsCount; ++row) {

            qint64 fileSize = resultsModel->item(row, 5)->data(Qt::UserRole + 1).toLongLong();
            foundFilesSize += fileSize;

            smallestFileSize = std::min(smallestFileSize, fileSize);
            biggestFileSize = std::max(biggestFileSize, fileSize);

            QDateTime modifiedTime = resultsModel->index(row, 8).data(Qt::UserRole).toDateTime();
            updateDateRange(modifiedTime, oldestModified, newestModified);

            QDateTime createdTime = resultsModel->index(row, 7).data(Qt::UserRole).toDateTime();
            updateDateRange(createdTime, oldestCreated, newestCreated);

            QDateTime accessedTime = resultsModel->index(row, 9).data(Qt::UserRole).toDateTime();
            updateDateRange(accessedTime, oldestAccessed, newestAccessed);

            int occurrences = resultsModel->item(row, 10)->data(Qt::UserRole + 1).toInt();
            totalOccurrences += occurrences;
            biggestOccurrences = std::max(biggestOccurrences, occurrences);
        }


        // --------------------------
        // Append general statistics
        // --------------------------
        appendNew("Start time", m_statsStartTime);
        appendNew("End time", m_statsEndTime);
        appendNew("Elapsed time", m_statsElapsed);


        const QStandardItem *searchTextItem = resultsModel->item(0, 11);
        const QString searchText = searchTextItem->data(Qt::UserRole + 2).toString();
        const bool isCaseInsensitive = searchTextItem->data(Qt::UserRole + 1).toString() == "1";
        const bool matchText = searchTextItem->data(Qt::UserRole + 3).toString() == "1";

        appendNew("Text to search", searchText);
        appendNew("Case sensitive", isCaseInsensitive ? "No" : "Yes");
        appendNew("Match text", matchText ? "No" : "Yes");


        // --------------------------
        // Append statistics from `m_statisticsMap`
        // --------------------------
        appendNew("Processed directories", QString::number(m_statisticsMap.value("Processed Directories")));
        appendNew("Processed files", QString::number(m_statisticsMap.value("Processed Files")));


        // --------------------------
        //
        // --------------------------
        appendNew("Occurrences found", QString::number(totalOccurrences));
        
        appendNew("Files found", QString::number(rowsCount));

        appendNew("Total found files size", QString("%1 Bytes").arg(foundFilesSize));
        if (foundFilesSize >= 1000)
            appendNew("Total found files size", Size_Utils::convertSizeToHuman(foundFilesSize, "SI"));

        appendNew("Smallest file size", QString("%1 Bytes").arg(smallestFileSize));
        if (smallestFileSize >= 1000)
            appendNew("Smallest file size", Size_Utils::convertSizeToHuman(smallestFileSize, "SI"));

        appendNew("Biggest file size", QString("%1 Bytes").arg(biggestFileSize));
        if (biggestFileSize >= 1000)
            appendNew("Biggest file size", Size_Utils::convertSizeToHuman(biggestFileSize, "SI"));

        appendNew("Largest matches per file", QString::number(biggestOccurrences));


        // --------------------------
        // Append date range statistics
        // --------------------------
        appendNew("Oldest modified file", oldestModified.toString("yyyy-MM-dd hh:mm:ss"));
        appendNew("Newest modified file", newestModified.toString("yyyy-MM-dd hh:mm:ss"));
        appendNew("Oldest file was created", oldestCreated.toString("yyyy-MM-dd hh:mm:ss"));
        appendNew("Newest file was created", newestCreated.toString("yyyy-MM-dd hh:mm:ss"));
        appendNew("Oldest file was accessed", oldestAccessed.toString("yyyy-MM-dd hh:mm:ss"));
        appendNew("Newest file was accessed", newestAccessed.toString("yyyy-MM-dd hh:mm:ss"));
    }




    QVariant data(const QModelIndex &index, int role) const override {
        if (!index.isValid())
            return QVariant(role);

        // Special handling for tooltips
        if (role == Qt::ToolTipRole && showTooltips) {
            if ((index.row() == 4 || index.row() == 5) && index.column() == 1)
                return QStandardItemModel::data(index, Qt::DisplayRole);

            // For other cells, let the base class handle tooltips
            return QStandardItemModel::data(index, role);
        }

        // For all other roles, use the base class implementation
        return QStandardItemModel::data(index, role);
    }


    // Setter to enable/disable tooltips dynamically
    void setTooltipsEnabled(bool enabled) {
        showTooltips = enabled;
    }


private:
    bool showTooltips;  // Flag to control whether tooltips are enabled

};
