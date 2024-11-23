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

#include <QDesktopServices>
#include <QMessageBox>
#include <QUrl>
#include <QStringList>
#include <QFileInfo>


class OpenFiles {

public:

    /**
     * Opens a single file specified by `filePath`.
     * Checks if the file exists, whether it exceeds the specified `maxFileSize`,
     * and confirms with the user before opening large files.
     *
     * @param filePath - The path to the file to be opened.
     * @param maxFileSize - The maximum allowed file size (in bytes) to open.
     * @param parent - An optional parent widget for dialog boxes.
     */
    static void openFile(const QString &filePath, const qint64 maxFileSize, QWidget *parent = nullptr) {

        // Check if the provided file path is empty.
        if (filePath.isEmpty()) {
            showWarning(parent, "No file path available.");
            return;
        }

        QFileInfo fileInfo(filePath);
        if (!fileInfo.exists()) {
            showWarning(parent, "The specified file does not exist.");
            return;
        }

        // Check the file size and confirm if it exceeds the maximum size.
        if (fileInfo.size() > maxFileSize && !confirmOpen(parent, 1, fileInfo.size()))
            return;

        QUrl fileUrl = QUrl::fromLocalFile(filePath);

        // Attempt to open the file using the default application.
        if (!QDesktopServices::openUrl(fileUrl))
            showWarning(parent, "Could not open the file.");

    }


    /**
     * Opens multiple files specified in `filesToOpen`.
     * Ensures that the number of files does not exceed `maxFilesToOpen` and that
     * each file is within the allowed size (`maxFileSize`). Prompts the user with
     * a confirmation dialog if limits are exceeded.
     *
     * @param filesToOpen - A QStringList of file paths to be opened.
     * @param maxFileSize - The maximum allowed file size (in bytes) to open.
     * @param maxFilesToOpen - The maximum number of files the user can open at once.
     * @param parent - An optional parent widget for dialog boxes.
     */
    static void openFiles(const QStringList &filesToOpen, const qint64 maxFileSize, const int maxFilesToOpen, QWidget *parent = nullptr) {

        // Check for empty file list first.
        if (filesToOpen.isEmpty()) {
            showWarning(parent, "No valid files found to open.");
            return;
        }

        // Confirm for the total number of files.
        if (filesToOpen.size() > maxFilesToOpen && !confirmOpen(parent, filesToOpen.size()))
            return;

        // Iterate through each file in the list.
        for (const QString &filePath : filesToOpen) {
            QFileInfo fileInfo(filePath);

            if (!fileInfo.exists()) {
                showWarning(parent, "The specified file does not exist: " + filePath);
                continue; // Skip to the next file if it doesn't exist.
            }

            // Check individual file size.
            if (fileInfo.size() > maxFileSize && !confirmOpen(parent, 1, fileInfo.size())) {
                return;
            }

            // Open the file.
            QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
        }
    }


private:
    /**
     * Shows a warning message dialog with the specified `message`.
     *
     * @param parent - The parent widget for the warning dialog.
     * @param message - The warning message to be displayed.
     */
    static void showWarning(QWidget *parent, const QString &message) {
        QMessageBox::warning(parent, "Warning", message);
    }


    /**
     * Displays a confirmation dialog to the user before opening files that may
     * impact system performance due to file size or quantity.
     *
     * @param parent - The parent widget for the confirmation dialog.
     * @param count - The number of files that the user is attempting to open.
     * @param fileSize - The size of the file (in bytes) if checking a single large file (optional).
     * @return bool - Returns true if the user confirms to proceed, false otherwise.
     */
    static bool confirmOpen(QWidget *parent, int count, qint64 fileSize = 0) {
        // Create a confirmation message for opening files.
        QString message = QString("Opening %1 file%2 may slow down or freeze your system. Do you want to continue?")
                              .arg(count)
                              .arg(count > 1 ? "s" : "");

        // Add note on file size if applicable.
        if (fileSize > 0)
            message += QString("<br>Note: The file size is %1 bytes.").arg(fileSize);

        // Show the confirmation dialog.
        QMessageBox::StandardButton reply = QMessageBox::question(
            parent,
            "Confirm Open",
            message,
            QMessageBox::Yes | QMessageBox::No
            );

        return (reply == QMessageBox::Yes);
    }
};
