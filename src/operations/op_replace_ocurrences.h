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

#include "components/replacement_dialog.h"
#include "components/scrollable_messagebox.h"
#include "utils/logger_utils.h"

#include <QInputDialog>
#include <QProgressDialog>
#include <QStringList>


/**
 * The ReplaceOccurrences class provides functionality for replacing text patterns in multiple files.
 * It includes options for user confirmation, progress feedback, and optional backup before replacement.
 */
class ReplaceOccurrences {


public:

    /**
     * Replaces occurrences of a specified text pattern in a list of files.
     * This function provides options for logging, creating backups, and user confirmation dialogs.
     *
     * @param filesToReplace - A QStringList of file paths where replacements will be made.
     * @param searchTextPattern - A QRegularExpression pattern to search for in each file.
     * @param enableLoggers - Boolean flag to enable logging of replacement actions.
     * @param loggersFilesToKeep - The number of log files to retain.
     * @param parent - An optional parent widget for dialog boxes.
     * @return QStringList - A list of file paths where replacements were successfully made.
     */
    static QStringList replaceContent(const QStringList &filesToReplace, const QRegularExpression &searchTextPattern,
                                      const bool enableLoggers, const int loggersFilesToKeep, QWidget *parent = nullptr) {

        QStringList successfullyModifiedFiles;

        // Check if there are files to replace; if not, show a warning and return.
        if (filesToReplace.isEmpty()) {
            QMessageBox::warning(parent, "Warning", "No files provided for replacement.");
            return successfullyModifiedFiles;
        }


        // ------------------------
        // ------------------------
        ReplacementDialog dialog(filesToReplace.size(), parent);

        if (dialog.exec() != QDialog::Accepted) {
            QMessageBox::information(parent, "Cancelled", "Replacement operation cancelled.");
            return successfullyModifiedFiles;
        }

        const QString strReplacement = dialog.getReplacementText();
        bool backup = dialog.isBackupSelected();
        bool ignoreErrors = dialog.isIgnoreErrorsChecked();


        // ------------------------
        // ------------------------
        // Initialize progress dialog to track the replacement process.
        QProgressDialog progress("Replacing occurrences in files...", "Cancel", 0, filesToReplace.size(), parent);
        progress.setWindowModality(Qt::WindowModal);

        // List to track files that failed during replacement.
        QStringList failedFiles;


        Logger logger(LoggerType::REPLACE, loggersFilesToKeep);


        // ------------------------
        // ------------------------
        // Iterate through each file to perform the replacement.
        for (int i = 0; i < filesToReplace.size(); ++i) {
            const QString &filePath = filesToReplace[i];

            // Open the file for reading using QFile and default encoding (UTF-8).
            QFile file(filePath);
            if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                // If file cannot be opened, add it to failed list if errors are not ignored.
                if (!ignoreErrors)
                    failedFiles.append(filePath);

                continue;
            }

            QTextStream in(&file);
            // In case you want to set a specific encoding (UTF-8, for instance):
            in.setEncoding(QStringConverter::Utf8);

            QString fileContent = in.readAll();
            file.close();

            // If the content is empty, add to failed files if errors are not ignored.
            if (fileContent.isEmpty()) {
                if (!ignoreErrors)
                    failedFiles.append(filePath);

                continue; // Skip to the next file.
            }

            // Perform the text replacement using the provided regular expression.
            QString newContent = fileContent.replace(searchTextPattern, strReplacement);


            // Handle optional backup before writing changes.
            if (backup && !backupFile(ignoreErrors, failedFiles, filePath))
                continue;


            // Open the file for writing using QFile and default encoding (UTF-8).
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                if (!ignoreErrors)
                    failedFiles.append(filePath);

                if (enableLoggers)
                    logger.log("Failure", "Replacement", filePath);

                continue; // Skip to next file if file cannot be opened for writing.
            }

            QTextStream out(&file);
            out.setEncoding(QStringConverter::Utf8); // Optionally, set UTF-8 explicitly (or remove if you want system default).
            out << newContent;
            file.close();

            // If the write was successful, add to the list of successfully modified files.
            successfullyModifiedFiles.append(filePath);

            // Log the success if logging is enabled.
            if (enableLoggers)
                logger.log("Success", "Replacement", filePath);

            // Update the progress dialog with the current progress.
            progress.setValue(i + 1);

            // Check if the user has cancelled the operation.
            if (progress.wasCanceled()) {
                QMessageBox::information(parent, "Operation Cancelled", "Replacement operation was cancelled.");
                return successfullyModifiedFiles;
            }
        }


        // ------------------------
        // ------------------------
        // After all replacements, provide feedback to the user.
        if (!failedFiles.isEmpty()) {
            ScrollableMessageBox msgBox("Replacement Incomplete",
                                        "Failed to replace occurrences in the following files:<br>",
                                        failedFiles, parent);
            msgBox.exec();

        } else {
            QMessageBox::information(parent, "Success", "Replacement completed successfully in all files.");
        }

        return successfullyModifiedFiles;
    }



private:
    /**
    * Define the max filename length constraint (260 on many systems). Adjust to 255 if you want a more universal limit
    */
    inline static const int MAX_FILENAME_LENGTH = 255;


    /**
     * Creates a backup of the specified file, appending a timestamp to the filename.
     * Backup creation fails if the system limits for filename length are exceeded.
     *
     * @param ignoreErrors - Boolean flag to indicate if errors should be ignored.
     * @param failedFiles - A reference to QStringList tracking files that failed backup.
     * @param filePath - The original file path to back up.
     * @return bool - True if the backup was created successfully, false otherwise.
     */
    inline static bool backupFile(bool ignoreErrors, QStringList failedFiles, const QString &filePath) {

        // Generate timestamp in the format yyyyMMdd_HHmmss
        const QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss");

        // Get the base file name and its directory
        QFileInfo fileInfo(filePath);
        const QString baseName = fileInfo.completeBaseName();
        const QString extension = fileInfo.suffix();
        const QString suffix = QString("_%1%2").arg(timestamp, extension.isEmpty() ? "" : "." + extension);


        // Truncate base name if necessary
        const QString truncatedBaseName = baseName.length() + suffix.length() > MAX_FILENAME_LENGTH
                                              ? QStringView(baseName).first(MAX_FILENAME_LENGTH - suffix.length()).toString()
                                              : baseName;

        const QString backupFilePath = QDir(fileInfo.path()).filePath(truncatedBaseName + suffix);

        // Try to copy the original file to the new backup location with the timestamped filename.
        if (!QFile::copy(filePath, backupFilePath)) {

            if (!ignoreErrors)
                failedFiles.append(filePath);

            return false; // Skip to the next file.
        }

        return true;
    }


};
