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

#include "models/standardmodel.h"
#include "utils/clipboard_utils.h"

#include <QFileDialog>
#include <QMessageBox>


class Directories_Utils {

public:

    /**
     * Opens a dialog for the user to select a directory and adds the selected directory to the provided model.
     * The dialog allows the user to choose a directory (not individual files), and only shows directories.
     *
     * @param directoriesModel - Pointer to the StandardModel object where the directory will be added.
     * @param parent - Optional parent widget for the QFileDialog, allowing integration into a parent window.
     */
    static void addFromModel(StandardModel *&directoriesModel, QWidget *parent = nullptr) {

        QFileDialog openDialog(parent);                // Initialize the file dialog with an optional parent
        openDialog.setFileMode(QFileDialog::Directory); // Set the dialog to directory selection mode
        openDialog.setOptions(QFileDialog::DontUseNativeDialog |
                              QFileDialog::ShowDirsOnly |
                              QFileDialog::DontResolveSymlinks); // Configure dialog options

        // If the dialog is not accepted (no selection made), exit function
        if (openDialog.exec() != QDialog::Accepted)
            return;

        QStringList selectedFiles = openDialog.selectedFiles();
        QString selectedDirectory = selectedFiles.first();

        directoriesModel->appendNewItem(selectedDirectory, false, true);
    }


    /**
     * Removes selected directories from the model after confirming with the user.
     * Prompts a warning if no directories are selected, and a confirmation dialog if there are selections.
     *
     * @param selectedIndexes - List of selected indexes to remove from the model.
     * @param directoriesModel - Pointer to the model from which directories will be removed.
     * @param parent - Optional parent widget for message boxes to integrate with a parent window.
     */
    static void removeFromModel(QModelIndexList &selectedIndexes, StandardModel *&directoriesModel, QWidget *parent = nullptr) {

        // If no items are selected, show a warning and exit
        if (selectedIndexes.isEmpty()) {
            QMessageBox::warning(parent, "Warning", "No directories are selected for deletion.");
            return;
        }

        // Confirmation prompt to proceed with deletion, adjusted for singular/plural language
        QMessageBox::StandardButton reply = QMessageBox::question(
            parent,
            "Confirm Deletion",
            QString("You are about to delete %1 director%2. Do you want to continue?")
                .arg(selectedIndexes.size())
                .arg(selectedIndexes.size() == 1 ? "y" : "ies"),
            QMessageBox::Yes | QMessageBox::No
            );

        // If the user selects "No", do not proceed with deletion
        if (reply == QMessageBox::No)
            return;

        // Collect rows to delete in a list
        QList<int> rowsToRemove;
        for (const QModelIndex &index : selectedIndexes)
            rowsToRemove.append(index.row());

        directoriesModel->removeItems(rowsToRemove); // Remove the selected rows from the model
    }


    /**
     * Copies the paths of the selected directories to the system clipboard.
     * For each selected directory, it fetches data from a specific column and copies it to the clipboard.
     *
     * @param selectedIndexes - List of selected indexes to copy paths from.
     * @param directoriesModel - Pointer to the model containing directory paths.
     */
    static void copyToClipboard(QModelIndexList &selectedIndexes, StandardModel *&directoriesModel) {

        QStringList directoriesList; // List to store paths of selected directories

        // Loop through each selected index, fetching data from the third column (index 2)
        for (const QModelIndex &index : selectedIndexes) {
            QModelIndex secondColumnIndex = directoriesModel->index(index.row(), 2);
            directoriesList.append(directoriesModel->data(secondColumnIndex).toString()); // Collect directory paths
        }

        // Copy the list of directory paths to the clipboard
        Clipboard_Utils::copyTextListToClipboard(directoriesList);
    }

};
