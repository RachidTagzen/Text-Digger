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

#include "appsettings.h"
#include "models/results_model.h"
#include "utils/datetime_utils.h"


#include <QMessageBox>
#include <QFileDialog>
#include <QProgressDialog>



const int UPDATE_PROGRESS_FREQUENCY = 600;  // Update the UI every 10 records for efficiency


class HandleResults {

public slots:

    static void importResults(ResultsModel &resultsModel, App_Settings &appSettings, QWidget *parent = nullptr) {
        // Timer to measure the duration of the import process
        QElapsedTimer elapsedTimer;
        elapsedTimer.start();


        // --------------------------
        // Open the file dialog to import the CSV file
        // --------------------------
        const QString initialDir = QDir(appSettings.getLastResultsDirectory()).exists() ?
                                       appSettings.getLastResultsDirectory() :
                                       HOME_DIRECTORY.absolutePath();

        QString filePath = QFileDialog::getOpenFileName(parent, "Import Results", initialDir, "CSV Files (*.csv)");

        // Exit early if no file was selected
        if (filePath.isEmpty())
            return;

        // Clear any existing data in the results model before importing new data
        resultsModel.clearModel();


        // --------------------------
        // Update the last results directory in app settings
        // --------------------------
        const QString lastResultsDirectory = QFileInfo(filePath).absolutePath();
        appSettings.setLastResultsDirectory(lastResultsDirectory);


        // --------------------------
        // Open and count total lines in the file to set the progress bar range
        // --------------------------
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(parent, "Error", "Unable to open the file for reading.");
            return;
        }

        int totalLines = 0;
        while (!file.atEnd()) {
            if (file.readLine().trimmed().isEmpty())
                continue;

            ++totalLines;
        }

        totalLines--; // Exclude the header line

        // --------------------------
        // Go back to the beginning of the file
        // --------------------------
        file.seek(0);


        // --------------------------
        // Initialize text stream for reading and set UTF-8 encoding
        // --------------------------
        QTextStream in(&file);
        in.setEncoding(QStringConverter::Utf8);


        // --------------------------
        // Read the first line (header) and validate it
        // --------------------------
        QString headerLine = in.readLine();
        if (headerLine.isEmpty()) {
            QMessageBox::critical(parent, "Error", "The file is empty or invalid.");
            file.close();
            return;
        }

        // --------------------------
        // Validate the header format against expected CSV_HEADER
        // --------------------------
        QStringList actualHeader = parseCsvLine(headerLine, CSV_HEADER.size());

        if (actualHeader != CSV_HEADER) {
            QMessageBox::critical(parent, "Error", "The file header does not match the expected format.");
            file.close();
            return;
        }


        // --------------------------
        // Setup progress dialog for feedback during import
        // --------------------------
        QProgressDialog progress("0 of 0 records", "Cancel", 0, totalLines, parent);
        progress.setWindowTitle("Importing data");
        progress.setWindowModality(Qt::WindowModal);
        progress.setRange(0, 0); // Indeterminate until progress starts

        QSet<QString> uniqueFiles;
        bool canceled = false;
        int currentRow = 0;

        resultsModel.startReset(); // Prepare model for bulk updates

        // --------------------------
        // Parse CSV rows and populate the model
        // --------------------------
        while (!in.atEnd()) {

            if (progress.wasCanceled()) {
                canceled = true;
                break;
            }

            QString line = in.readLine().trimmed();

            if (line.isEmpty())
                continue;

            // Parse the line and validate the number of columns
            const QStringList columns = parseCsvLine(line, CSV_HEADER.size());

            if (columns.size() != CSV_HEADER.size())
                continue;  // Skip rows that don't have the expected number of columns


            // Skip duplicate file paths
            const QString filePath = columns[2];

            if (uniqueFiles.contains(filePath)) {
                currentRow++;
                if (currentRow % UPDATE_PROGRESS_FREQUENCY == 0) {
                    progress.setValue(currentRow);
                    progress.setLabelText(QString("%1 of %2 records").arg(currentRow).arg(totalLines));
                }

                continue;
            }

            // Extract and format data for each column
            const Qt::CheckState checkedStatus = columns[0] == "x" ? Qt::Checked : Qt::Unchecked;
            const QString filename = columns[1];
            const qint64 fileSize = columns[3].toLongLong();
            const QString sizeSystem = columns[4];
            const QString mimeType = columns[5];
            const QDateTime created = QDateTime::fromString(columns[6], Qt::ISODate);
            const QDateTime lastModification = QDateTime::fromString(columns[7], Qt::ISODate);
            const QDateTime lastAccess = QDateTime::fromString(columns[8], Qt::ISODate);
            const int foundOccurrences = columns[9].toInt();

            QSet<int> linesNumbers;
            const QStringList lineNumbers = columns[10].split("-", Qt::SkipEmptyParts);
            for (const QString &lineNumber : lineNumbers)
                linesNumbers.insert(lineNumber.toInt());


            const bool matchText = columns[11] == "true" || columns[11] == "1";
            const QString searchText = columns[12];
            const QString searchTextPatternOption = columns[13];

            // Create and format table items
            QStandardItem *uuidItem = new QStandardItem(QUuid::createUuid().toString());
            uuidItem->setTextAlignment(Qt::AlignCenter);


            // Determine the appropriate icon for the file based on type (file or folder)
            QFileInfo fileInfo(filePath);
            QIcon fileIcon;
            if (fileInfo.isDir())
                fileIcon = QIcon::fromTheme("folder", QApplication::style()->standardIcon(QStyle::SP_DirIcon));
            else
                fileIcon = QIcon::fromTheme(fileInfo.suffix(), QApplication::style()->standardIcon(QStyle::SP_FileIcon));

            // Icon item for displaying the file/folder icon, center-aligned
            QStandardItem *iconItem = new QStandardItem();
            iconItem->setData(fileIcon, Qt::DecorationRole);
            iconItem->setTextAlignment(Qt::AlignCenter);
            // iconItem->setTextAlignment(Qt::AlignCenter | static_cast<Qt::AlignmentFlag>(16));  // Center-align with additional flag


            // Checkbox item for user selection, center-aligned and unchecked by default
            QStandardItem *item_checkbox = new QStandardItem();
            item_checkbox->setFlags(item_checkbox->flags() | Qt::ItemIsUserCheckable);
            item_checkbox->setTextAlignment(Qt::AlignCenter);
            item_checkbox->setCheckable(true);
            item_checkbox->setEditable(false);
            item_checkbox->setCheckState(checkedStatus);

            // File name item (file or directory name)
            QStandardItem *filenameItem = new QStandardItem(filename);

            // File path item (full file path)
            QStandardItem *pathItem = new QStandardItem(filePath);

            // File size item in human-readable format, right-aligned
            QStandardItem *sizeItem = new QStandardItem(Size_Utils::convertSizeToHuman(fileSize, sizeSystem));
            sizeItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            sizeItem->setData(fileSize, Qt::UserRole + 1);  // Store actual size as qint64 for potential sorting
            sizeItem->setData(sizeSystem, Qt::UserRole + 2);  // Store Size System

            // MIME type item for displaying the file's MIME type
            QStandardItem *mimeTypeItem = new QStandardItem(mimeType);

            // Created timestamp item, center-aligned and formatted
            QStandardItem *createdItem = new QStandardItem(created.toString("yyyy-MM-dd hh:mm:ss"));
            createdItem->setTextAlignment(Qt::AlignCenter);
            createdItem->setData(created, Qt::UserRole);  // Store QDateTime for sorting purposes

            // Last modified timestamp item, center-aligned and formatted
            QStandardItem *modifiedItem = new QStandardItem(lastModification.toString("yyyy-MM-dd hh:mm:ss"));
            modifiedItem->setTextAlignment(Qt::AlignCenter);
            modifiedItem->setData(lastModification, Qt::UserRole);  // Store QDateTime for sorting purposes

            // Last accessed timestamp item, center-aligned and formatted
            QStandardItem *accessedItem = new QStandardItem(lastAccess.toString("yyyy-MM-dd hh:mm:ss"));
            accessedItem->setTextAlignment(Qt::AlignCenter);
            accessedItem->setData(lastAccess, Qt::UserRole);  // Store QDateTime for sorting purposes

            // Occurrences item indicating how many times a term was found, center-aligned
            QStandardItem *occurrencesItem = new QStandardItem(QString::number(foundOccurrences));
            occurrencesItem->setTextAlignment(Qt::AlignCenter);
            occurrencesItem->setData(foundOccurrences, Qt::UserRole + 1);
            occurrencesItem->setData(QVariant::fromValue(linesNumbers), Qt::UserRole + 2);  // Store set of line numbers for reference

            QStandardItem *searchTextPatternItem = new QStandardItem(searchText);
            searchTextPatternItem->setData(searchTextPatternOption, Qt::UserRole + 1);
            searchTextPatternItem->setData(searchText, Qt::UserRole + 2);
            searchTextPatternItem->setData(matchText, Qt::UserRole + 3);


            // --------------------------
            // Add the row to the model with all created items
            // --------------------------
            resultsModel.appendRow({uuidItem,
                                    iconItem,
                                    item_checkbox,
                                    filenameItem,
                                    pathItem,
                                    sizeItem,
                                    mimeTypeItem,
                                    createdItem,
                                    modifiedItem,
                                    accessedItem,
                                    occurrencesItem,
                                    searchTextPatternItem});


            uniqueFiles.insert(filePath);

            currentRow++;
            if (currentRow % UPDATE_PROGRESS_FREQUENCY == 0) {
                progress.setValue(currentRow);
                progress.setLabelText(QString("%1 of %2 records").arg(currentRow).arg(totalLines));
            }

        }

        // --------------------------
        // Finalize import and notify user
        // --------------------------
        file.close();
        progress.close();


        resultsModel.finishReset();  // Notify views that the model update is complete, prompting a full refresh


        const QString totalElapsedTime = DateTime_Utils::formatElapsedTime(elapsedTimer.elapsed());

        if (canceled)
            QMessageBox::warning(parent,
                                 "Import Canceled",
                                 QString("The import was canceled. Partial data has been imported.\n\nElapsed time: %1").arg(totalElapsedTime));
        else
            QMessageBox::information(parent,
                                     "Success",
                                     QString("Import completed successfully.\n\nElapsed time: %1").arg(totalElapsedTime));

    }




    static void exportResults(const ResultsModel &resultsModel, App_Settings &appSettings, QWidget *parent = nullptr) {

        QElapsedTimer elapsedTimer;
        elapsedTimer.start();


        const int rowsCount = resultsModel.rowCount();

        if (rowsCount == 0) {
            QMessageBox::warning(parent, "No Results", "There are no results to export.");
            return;
        }


        // --------------------------
        // Open the file dialog to save the CSV file
        // --------------------------
        const QString initialDir = QDir(appSettings.getLastResultsDirectory()).exists()
                                       ? appSettings.getLastResultsDirectory()
                                       : HOME_DIRECTORY.absolutePath();

        QString filePath = QFileDialog::getSaveFileName(parent, "Export Results", initialDir, "CSV Files (*.csv)");

        if (filePath.isEmpty())
            return;


        // --------------------------
        // Last Results Directory
        // --------------------------
        const QString lastResultsDirectory = QFileInfo(filePath).absolutePath();
        appSettings.setLastResultsDirectory(lastResultsDirectory);


        // --------------------------
        // Ensure the file ends with .csv extension
        // --------------------------
        if (!filePath.endsWith(".csv", Qt::CaseInsensitive))
            filePath += ".csv";

        QFile file(filePath);

        // if (!file.permissions().testFlag(QFileDevice::WriteUser)) {
        //     QMessageBox::critical(parent, "Error", "You don't have write permissions for the selected file.");
        //     return;
        // }

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::critical(parent, "Error", "Could not open file for writing.");
            return;
        }

        QTextStream out(&file);
        out.setEncoding(QStringConverter::Utf8);


        // --------------------------
        // Write header row (skip the first two columns)
        // --------------------------
        out << "\"" << CSV_HEADER.join("\";\"") << "\"" << "\n";

        QProgressDialog progress("0 of 0 records", "Cancel", 0, rowsCount, parent);
        progress.setWindowTitle("Exporting data");
        progress.setWindowModality(Qt::WindowModal);

        // --------------------------
        // Write data rows
        // --------------------------
        bool canceled = false;
        int currentRow = 0;

        for (int row = 0; row < rowsCount; ++row) {

            if (progress.wasCanceled()) {
                canceled = true;
                break;
            }

            const QString checkedStatus = quoteAndEscape(resultsModel.item(row, 2)->checkState() != Qt::Unchecked ? "x" : "");
            const QString filename = quoteAndEscape(resultsModel.item(row, 3)->text());
            const QString filePath = quoteAndEscape(resultsModel.item(row, 4)->text());
            const QString fileSize = quoteAndEscape(resultsModel.item(row, 5)->data(Qt::UserRole + 1).toString());
            const QString sizeSystem = quoteAndEscape(resultsModel.item(row, 5)->data(Qt::UserRole + 2).toString());
            const QString mimeType = quoteAndEscape(resultsModel.item(row, 6)->text());
            const QString created = quoteAndEscape(resultsModel.item(row, 7)->text());
            const QString lastModification = quoteAndEscape(resultsModel.item(row, 8)->text());
            const QString lastAccess = quoteAndEscape(resultsModel.item(row, 9)->text());

            QStandardItem *occurrencesItem = resultsModel.item(row, 10);
            const QString foundOccurrences = quoteAndEscape(occurrencesItem ? occurrencesItem->text() : "");

            const QSet<int> lineNumbers_Set = occurrencesItem->data(Qt::UserRole + 2).value<QSet<int>>();
            QList<int> sortedList = lineNumbers_Set.values();
            std::sort(sortedList.begin(), sortedList.end());

            QStringList sortedLines;
            for (int number : sortedList)
                sortedLines.append(QString::number(number));

            const QString searchText = quoteAndEscape(resultsModel.item(row, 11)->text());
            const QString patternOption = quoteAndEscape(resultsModel.item(row, 11)->data(Qt::UserRole + 1).toString());
            const QString matchText = quoteAndEscape(resultsModel.item(row, 11)->data(Qt::UserRole + 3).toString());

            const QStringList record({checkedStatus,
                filename,
                filePath,
                fileSize,
                sizeSystem,
                mimeType,
                created,
                lastModification,
                lastAccess,
                foundOccurrences,
                quoteAndEscape(sortedLines.join("-")),
                matchText,
                searchText,
                patternOption
            });

            out << record.join(";") << "\n";

            currentRow++;
            if (currentRow % UPDATE_PROGRESS_FREQUENCY == 0) {
                progress.setValue(currentRow);
                progress.setLabelText(QString("%1 of %2 records").arg(currentRow).arg(rowsCount));
            }

        }

        file.close();
        progress.close();


        const QString totalElapsedTime = DateTime_Utils::formatElapsedTime(elapsedTimer.elapsed());

        if (canceled)
            QMessageBox::warning(parent,
                                 "Export Canceled",
                                 QString("The export was canceled. Partial data has been exported.\n\nElapsed time: %1").arg(totalElapsedTime));
        else
            QMessageBox::information(parent,
                                     "Success",
                                     QString("Exported to CSV successfully.\n\nElapsed time: %1").arg(totalElapsedTime));

    }


private:
    static QString quoteAndEscape(const QString &value) {
        QString result = value;
        result.replace("\"", "\"\"");
        return "\"" + result + "\"";
    }


    static QStringList parseCsvLine(const QString &line, const int expectedColumnCount, bool trimFields = true) {

        QStringList fields;
        fields.reserve(expectedColumnCount);

        // Type-safe field size estimation
        const qsizetype estimatedFieldSize = std::max<qsizetype>(64, line.length() / expectedColumnCount);
        QString current;
        current.reserve(estimatedFieldSize);

        const qsizetype length = line.length();
        bool inQuotes = false;

        // Track max field size for future optimizations
        qsizetype maxFieldSize = 0;

        for (qsizetype i = 0; i < length; ++i) {
            const QChar c = line[i];

            if (inQuotes) {
                if (c == '"') {
                    if (i + 1 < length && line[i + 1] == '"') {
                        current += c;
                        ++i;
                    } else {
                        inQuotes = false;
                    }
                } else {
                    current += c;
                }
            } else {
                if (c == '"') {
                    inQuotes = true;
                } else if (c == ';') {
                    // Update max field size statistics
                    maxFieldSize = std::max<qsizetype>(maxFieldSize, current.size());

                    // Optional trimming during parsing to avoid extra pass
                    if (trimFields) {
                        QStringView view(current);
                        if (view.size() != view.trimmed().size()) {
                            current = view.trimmed().toString();
                        }
                    }

                    fields.append(std::move(current));

                    // Adjust reserve size based on observed field sizes
                    current.clear();
                    current.reserve(std::min<qsizetype>(maxFieldSize + 16, estimatedFieldSize));
                } else {
                    current += c;
                }
            }
        }

        // Handle final field
        if (trimFields) {
            QStringView view(current);
            if (view.size() != view.trimmed().size())
                current = view.trimmed().toString();
        }

        fields.append(std::move(current));

        // Handle missing columns efficiently
        if (fields.size() < expectedColumnCount)
            fields.resize(expectedColumnCount);

        return fields;
    }


};
