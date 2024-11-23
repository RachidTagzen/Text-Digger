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

#include "enumerators/enums.h"
#include "components/scrollable_messagebox.h"
#include "utils/logger_utils.h"

#include <QMessageBox>
#include <QProgressDialog>
#include <QStringList>


// The DeleteFiles class provides a static method to delete a list of files with user confirmation and progress feedback.
class DeleteFiles {

public:
    /**
     * Deletes a list of files and returns the list of successfully deleted files.
     *
     * @param filesToDelete - A QStringList containing file paths to delete.
     * @param enableLoggers - A boolean flag to enable logging of deletion actions.
     * @param loggersFilesToKeep - The number of log files to retain when logging is enabled.
     * @param parent - An optional QWidget pointer for setting the parent of message boxes and dialogs.
     * @return QStringList - A list of files that were successfully deleted.
     */
    static QStringList deleteFiles(const QStringList &filesToDelete, const bool enableLoggers,
                                   const int loggersFilesToKeep, QWidget *parent = nullptr) {

        QStringList successfullyDeleted;

        // Check if there are files to delete.
        if (filesToDelete.isEmpty()) {
            QMessageBox::warning(parent, "Warning", "No valid files found to delete.");
            return successfullyDeleted;
        }

        // Create a confirmation message box to alert the user about the deletion.
        QMessageBox msgBox(parent);
        msgBox.setWindowTitle("Confirm Deletion");
        msgBox.setText(QString("You are about to <strong>permanently</strong> delete %1 file%2.<br>Do you want to proceed?")
                           .arg(filesToDelete.size())
                           .arg(filesToDelete.size() != 1 ? "s" : ""));
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

        // Add checkbox for "Ignore Errors and Continue"
        QCheckBox ignoreErrorsCheckBox("Ignore errors and continue", &msgBox);
        ignoreErrorsCheckBox.setChecked(false);
        msgBox.setCheckBox(&ignoreErrorsCheckBox);

        // Show the message box and check if the user confirms the deletion.
        if (msgBox.exec() != QMessageBox::Yes)
            return successfullyDeleted;


        bool ignoreErrors = ignoreErrorsCheckBox.isChecked();

        // Create a progress dialog to show deletion progress.
        QProgressDialog progress("Deleting files...", "Cancel", 0, filesToDelete.size(), parent);
        progress.setWindowModality(Qt::WindowModal);

        Logger logger(LoggerType::DELETE, loggersFilesToKeep);

        // Track any files that couldn't be deleted.
        QStringList failedDeletions;


        // Iterate over the list of files to delete.
        for (int i = 0; i < filesToDelete.size(); ++i) {
            const QString &filePath = filesToDelete[i];

            if (QFile::remove(filePath)) {
                successfullyDeleted.append(filePath);

                if (enableLoggers)
                    logger.log("Success", "Delete", filePath);

            } else if (!ignoreErrors) {
                failedDeletions.append(filePath);

                if (enableLoggers)
                    logger.log("Fail", "Delete", filePath);
            }

            // Update the progress dialog with the current progress.
            progress.setValue(i + 1);

            // Check if the user has canceled the deletion operation.
            if (progress.wasCanceled()) {
                if (enableLoggers)
                    logger.log("Info", "Cancel", "Deletion operation was cancelled by the user.");

                QMessageBox::information(parent, "Operation Cancelled", "Deletion operation was cancelled.");
                return successfullyDeleted;
            }
        }

        // After the deletion process, show a message box with the results.
        if (!failedDeletions.isEmpty()) {
            ScrollableMessageBox msgBox("Deletion Failed",
                                        "Failed to delete the following files:<br>",
                                        failedDeletions, parent);
            msgBox.exec();

        } else {
            QMessageBox::information(parent, "Success", "All selected files were successfully deleted.");
        }

        return successfullyDeleted;
    }


};
