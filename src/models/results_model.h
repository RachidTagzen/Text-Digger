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

#include "utils/size_utils.h"
#include "operations/op_rescan_occurrences.h"

#include <QApplication>
#include <QStandardItemModel>
#include <QMimeDatabase>
#include <QStyle>
#include <QUuid>
#include <QProgressDialog>
#include <QMessageBox>


class ResultsModel : public QStandardItemModel {
    Q_OBJECT


public:
    /**
     * Constructor for ResultsModel, initializes the model with a set column structure.
     * @param parent - The parent QObject, typically the parent widget or object.
     */
    ResultsModel(QObject *parent) : QStandardItemModel(parent) {

        setColumnCount(12);
        setHorizontalHeaderItem(0, new QStandardItem("Uuid"));
        setHorizontalHeaderItem(1, new QStandardItem(""));
        setHorizontalHeaderItem(2, new QStandardItem("√"));
        setHorizontalHeaderItem(3, new QStandardItem("File"));
        setHorizontalHeaderItem(4, new QStandardItem("Path"));
        setHorizontalHeaderItem(5, new QStandardItem("Size"));
        setHorizontalHeaderItem(6, new QStandardItem("MIME Type"));
        setHorizontalHeaderItem(7, new QStandardItem("Created"));
        setHorizontalHeaderItem(8, new QStandardItem("Modified"));
        setHorizontalHeaderItem(9, new QStandardItem("Accessed"));
        setHorizontalHeaderItem(10, new QStandardItem("Founds"));
        setHorizontalHeaderItem(11, new QStandardItem("Search Text Pattern"));
    }


    /**
     * Adds a new row to the model with file information.
     * @param fileInfo - Information about the file (e.g., size, timestamps).
     * @param filePath - The full path to the file.
     * @param mimeType - The MIME type of the file.
     * @param sizeSystem - The system to use for size conversion (e.g., "SI" or "IEC").
     * @param occurrences - The number of times a particular search term was found in the file.
     * @param linesNumbers - Set of line numbers where search terms were found.
     */
    void appendNew(const QFileInfo &fileInfo, const QString &filePath, const QString &mimeType, const QString &sizeSystem,
                   const int &occurrences, const QSet<int> &linesNumbers, const QRegularExpression &searchTextPattern,
                   const bool &matchText) {

        // --------------------------
        // Generate a unique identifier for the file and center-align it
        // --------------------------
        QStandardItem *uuidItem = new QStandardItem(QUuid::createUuid().toString());
        uuidItem->setTextAlignment(Qt::AlignCenter);

        // --------------------------
        //
        // --------------------------
        // Determine the appropriate icon for the file based on type (file or folder)
        QIcon fileIcon;
        if (fileInfo.isDir())
            fileIcon = QIcon::fromTheme("folder", QApplication::style()->standardIcon(QStyle::SP_DirIcon));
        else
            fileIcon = QIcon::fromTheme(fileInfo.suffix(), QApplication::style()->standardIcon(QStyle::SP_FileIcon));

        QStandardItem *iconItem = new QStandardItem();
        iconItem->setData(fileIcon, Qt::DecorationRole);
        iconItem->setTextAlignment(Qt::AlignCenter | static_cast<Qt::AlignmentFlag>(16));  // Center-align with additional flag
        iconItem->setFlags(iconItem->flags());  // Retain existing flags

        QStandardItem *item_checkbox = new QStandardItem();
        item_checkbox->setFlags(item_checkbox->flags() | Qt::ItemIsUserCheckable);
        item_checkbox->setTextAlignment(Qt::AlignCenter);
        item_checkbox->setCheckable(true);
        item_checkbox->setEditable(false);
        item_checkbox->setCheckState(Qt::Unchecked);

        QStandardItem *filenameItem = new QStandardItem(fileInfo.fileName());

        QStandardItem *pathItem = new QStandardItem(filePath);

        QStandardItem *sizeItem = new QStandardItem(Size_Utils::convertSizeToHuman(fileInfo.size(), sizeSystem));
        sizeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        sizeItem->setData(fileInfo.size(), Qt::UserRole + 1);  // Store actual size as qint64 for potential sorting
        sizeItem->setData(sizeSystem, Qt::UserRole + 2);  // Store the Szie system

        QStandardItem *mimeTypeItem = new QStandardItem(mimeType);

        QStandardItem *createdItem = new QStandardItem(fileInfo.birthTime().toString("yyyy-MM-dd hh:mm:ss"));
        createdItem->setTextAlignment(Qt::AlignCenter);
        createdItem->setData(fileInfo.birthTime(), Qt::UserRole);  // Store QDateTime for sorting purposes

        QStandardItem *modifiedItem = new QStandardItem(fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss"));
        modifiedItem->setTextAlignment(Qt::AlignCenter);
        modifiedItem->setData(fileInfo.lastModified(), Qt::UserRole);  // Store QDateTime for sorting purposes

        QStandardItem *accessedItem = new QStandardItem(fileInfo.lastRead().toString("yyyy-MM-dd hh:mm:ss"));
        accessedItem->setTextAlignment(Qt::AlignCenter);
        accessedItem->setData(fileInfo.lastRead(), Qt::UserRole);  // Store QDateTime for sorting purposes

        QStandardItem *occurrencesItem = new QStandardItem(QString::number(occurrences));
        occurrencesItem->setTextAlignment(Qt::AlignCenter);
        occurrencesItem->setData(occurrences, Qt::UserRole + 1);
        occurrencesItem->setData(QVariant::fromValue(linesNumbers), Qt::UserRole + 2);  // Store set of line numbers for reference

        QStandardItem *searchTextPatternItem = new QStandardItem(searchTextPattern.pattern());
        searchTextPatternItem->setData(QString::number(static_cast<int>(searchTextPattern.patternOptions()), 16), Qt::UserRole + 1);
        searchTextPatternItem->setData(searchTextPattern.pattern(), Qt::UserRole + 2);
        searchTextPatternItem->setData(matchText, Qt::UserRole + 3);


        // --------------------------
        // Add the row to the model with all created items
        // --------------------------
        appendRow({uuidItem, iconItem, item_checkbox, filenameItem, pathItem, sizeItem, mimeTypeItem, createdItem,
                   modifiedItem, accessedItem, occurrencesItem, searchTextPatternItem});
    }


    /**
     * Rescan the results
     */
    void rescan(const bool &fileReadingTimeout, const int &timeoutFileReading, const bool &limitOccurrencesFound,
                const int &occurrencesFoundLimit, const bool matchText, QWidget *parent = nullptr) {

        if (isEmpty())
            return;


        startReset();  // Notify views that the model is about to undergo major changes


        const QStandardItem *searchTextItem = item(0, 11);
        const QString searchText = searchTextItem->data(Qt::UserRole + 2).toString();

        QRegularExpression::PatternOptions patternOptions = QRegularExpression::NoPatternOption;

        if (searchTextItem->data(Qt::UserRole + 1).toString() == "1")
            patternOptions |= QRegularExpression::CaseInsensitiveOption;

        QRegularExpression searchTextPattern = QRegularExpression(searchText, patternOptions);


        // Create a progress dialog to show rescan progress.
        QProgressDialog progress("Rescan files...", "Cancel", 0, rowCount(), parent);
        progress.setWindowModality(Qt::WindowModal);


        const QMimeDatabase mimeDatabase;
        QVector<int> rowsToDelete;

        for (int row = 0; row < rowCount(); ++row) {

            // --------------------------
            //
            // --------------------------
            QString filePath = item(row, 4)->text();
            const QFileInfo fileInfo(filePath);

            if (!fileInfo.isFile()) {
                rowsToDelete.append(row);
                continue;
            }


            // --------------------------
            //
            // --------------------------
            QStandardItem *iconItem = item(row, 1);
            QStandardItem *sizeItem = item(row, 5);
            QStandardItem *mimeTypeItem = item(row, 6);
            QStandardItem *createdItem = item(row, 7);
            QStandardItem *modifiedItem = item(row, 8);
            QStandardItem *accessedItem = item(row, 9);
            QStandardItem *occurrencesItem = item(row, 10);


            // --------------------------
            //
            // --------------------------
            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                rowsToDelete.append(row);
                continue ;
            }

            const QPair<int, QSet<int>> occurencesFound = RescanOccurrences::scan(file, fileReadingTimeout,
                                                                                  timeoutFileReading, limitOccurrencesFound,
                                                                                  occurrencesFoundLimit, searchTextPattern,
                                                                                  progress.wasCanceled());

            file.close();

            // Add the result to the results QVector if any occurrences were found
            bool shouldAppend = (matchText && occurencesFound.first > 0) || (!matchText && occurencesFound.first == 0);

            if (shouldAppend) {
                occurrencesItem->setText(QString::number(occurencesFound.first));
                occurrencesItem->setData(occurencesFound.first, Qt::UserRole + 1);
                occurrencesItem->setData(QVariant::fromValue(occurencesFound.second), Qt::UserRole + 2);
            } else {
                rowsToDelete.append(row);
            }


            // --------------------------
            //
            // --------------------------
            // Determine the appropriate icon for the file based on type (file or folder)
            QIcon fileIcon;
            if (fileInfo.isDir())
                fileIcon = QIcon::fromTheme("folder", QApplication::style()->standardIcon(QStyle::SP_DirIcon));
            else
                fileIcon = QIcon::fromTheme(fileInfo.suffix(), QApplication::style()->standardIcon(QStyle::SP_FileIcon));

            iconItem->setData(fileIcon, Qt::DecorationRole);

            QString sizeSystem = sizeItem->data(Qt::UserRole + 2).toString();
            sizeItem->setText(Size_Utils::convertSizeToHuman(fileInfo.size(), sizeSystem));
            sizeItem->setData(fileInfo.size(), Qt::UserRole + 1);
            sizeItem->setData(sizeSystem, Qt::UserRole + 2);

            mimeTypeItem->setText(mimeDatabase.mimeTypeForFile(fileInfo).name());

            createdItem->setText(fileInfo.birthTime().toString("yyyy-MM-dd hh:mm:ss"));
            createdItem->setData(fileInfo.birthTime(), Qt::UserRole);

            modifiedItem->setText(fileInfo.lastModified().toString("yyyy-MM-dd hh:mm:ss"));
            modifiedItem->setData(fileInfo.lastModified(), Qt::UserRole);

            accessedItem->setText(fileInfo.lastRead().toString("yyyy-MM-dd hh:mm:ss"));
            accessedItem->setData(fileInfo.lastRead(), Qt::UserRole);


            // Update the progress dialog with the current progress.
            progress.setValue(row + 1);

            // Check if the user has canceled the rescan operation.
            if (progress.wasCanceled()) {
                QMessageBox::information(parent, "Operation Cancelled", "Rescan operation was cancelled.");
                // return successfullyDeleted;
                return;
            }

        }

        // Delete rows in reverse order from the collected list
        for (int i = rowsToDelete.size() - 1; i >= 0; --i)
            removeRow(rowsToDelete[i]);


        finishReset();  // Notify views that the model update is complete, prompting a full refresh

    }


    /**
     * Check if the model is empty.
     */
    bool isEmpty() {
        return rowCount() == 0;
    }


    /**
     * Clears all data from the model.
     */
    void clearModel() {
        removeRows(0, rowCount());
    }



    /**
     * @brief Initiates a model reset.
     *
     * This function emits the `beginResetModel()` signal, indicating to any connected views
     * that a significant change in the model's data is about to happen. Use this function
     * before making extensive modifications to the model data to ensure that views update
     * correctly. Always call `finishReset()` afterward to complete the reset.
     */
    void startReset() {
        beginResetModel();
    }


    /**
     * @brief Completes a model reset.
     *
     * This function emits the `endResetModel()` signal, notifying connected views that
     * the model reset has finished and that they should reload data. Ensure that
     * `startReset()` has been called before this function to maintain model-view consistency.
     */
    void finishReset() {
        endResetModel();
    }


};
