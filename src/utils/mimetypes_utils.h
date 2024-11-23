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

#include "components/scrollable_messagebox.h"
#include "models/standardmodel.h"
#include "utils/clipboard_utils.h"
#include "utils/file_utils.h"

#include <QInputDialog>
#include <QMessageBox>
#include <QMimeDatabase>


class MimeTypes_Utils {


public:

    /**
     * Prompts the user to input MIME types and adds them to the provided model.
     * The input is expected to be a comma-separated string.
     * @param mimetypesModel - Reference to the model where MIME types will be added.
     * @param parent - Optional parent widget for the dialog.
     */
    static void addToModel(StandardModel *&mimetypesModel, QWidget *parent = nullptr) {
        bool ok;
        QString text = QInputDialog::getText(parent,
                                             "Add MIME Types", "MIME Types (comma-separated):",
                                             QLineEdit::Normal,
                                             "",
                                             &ok);

        if (!ok)
            return;

        processMimeTypes(text, mimetypesModel, parent);
    }

    /**
     * Deletes selected MIME types from the model after user confirmation.
     * @param selectedIndexes - List of selected indexes to be deleted.
     * @param mimetypesModel - Reference to the model from which MIME types will be removed.
     * @param parent - Optional parent widget for the message box.
     */
    static void deleteSelected(QModelIndexList &selectedIndexes, StandardModel *&mimetypesModel, QWidget *parent = nullptr) {

        if (selectedIndexes.isEmpty()) {
            QMessageBox::warning(parent, "Warning", "No MIME Types are selected for deletion.");
            return;
        }

        // Ask for confirmation before deleting
        QMessageBox::StandardButton reply = QMessageBox::question(
            parent,
            "Confirm Deletion",
            QString("You are about to delete %1 MIME type%2. Do you want to continue?")
                .arg(selectedIndexes.size())
                .arg(selectedIndexes.size() == 1 ? "" : "s"),
            QMessageBox::Yes | QMessageBox::No
            );

        if (reply == QMessageBox::No)
            return;

        // Collect the rows to be removed from the model
        QList<int> rowsToRemove;
        for (const QModelIndex &index : selectedIndexes)
            rowsToRemove.append(index.row());


        // Remove the selected rows from the model
        mimetypesModel->removeItems(rowsToRemove);
    }

    /**
     * Restores MIME types from a string representation, updating the model accordingly.
     * @param mimeTypesData - A string containing MIME types to restore.
     * @param mimetypesModel - Reference to the model where MIME types will be added.
     * @param parent - Optional parent widget for displaying error messages.
     */
    static void restoreFromDatabase(const QString &mimeTypesData, StandardModel *&mimetypesModel, QWidget *parent = nullptr) {

        if (mimeTypesData.isEmpty())
            return;

        // Split the MIME types using regex to handle newlines and trim spaces
        QStringList mimeTypesList = mimeTypesData.split(SEPARATOR_WITHOUT_WHITESPACE_REGEX, Qt::SkipEmptyParts);

        QStringList unsupportedMimeTypes;
        QMimeDatabase mimeDatabase;

        // Process each MIME type
        for (QString &part : mimeTypesList) {
            part = part.trimmed();
            QString mimetype = part.mid(3).trimmed();

            // Check if the MIME type is valid
            QMimeType qmimeType = mimeDatabase.mimeTypeForName(mimetype);

            if (qmimeType.isValid())
                mimetypesModel->appendNewItem(qmimeType.name(), part.startsWith("[1]"), true);
            else
                unsupportedMimeTypes << mimetype;
        }

        mimetypesModel->sort(2);


        // Show a message box if there are unsupported MIME types
        if (!unsupportedMimeTypes.isEmpty()) {
            ScrollableMessageBox msgBox("Invalid MIME Types",
                                        "The following MIME types aren't valid in Qt:\n",
                                        unsupportedMimeTypes, parent); // Use parent if provided
            msgBox.exec();
        }
    }

    /**
     * Copies the directories associated with selected MIME types to the clipboard.
     * @param selectedIndexes - List of selected indexes to copy.
     * @param model - Reference to the model from which to copy directories.
     */
    static void copyToClipboard(QModelIndexList &selectedIndexes, StandardModel *&model) {

        QStringList directoriesList;

        for (const QModelIndex &index : selectedIndexes) {
            QModelIndex secondColumnIndex = model->index(index.row(), 2);
            directoriesList.append(model->data(secondColumnIndex).toString());
        }

        Clipboard_Utils::copyTextListToClipboard(directoriesList);
    }

    /**
     * Pastes MIME types from the clipboard into the provided model.
     * @param mimetypesModel - Reference to the model where MIME types will be pasted.
     * @param parent - Optional parent widget for the dialog.
     */
    static void pasteFromClipboard(StandardModel *&mimetypesModel, QWidget *parent = nullptr) {
        processMimeTypes(Clipboard_Utils::pasteClipboardToPlainText(), mimetypesModel, parent);
    }

    /**
     * Resets the MIME types in the model to a predefined state.
     * @param model - Reference to the model to be reset.
     * @param parent - Optional parent widget for the confirmation dialog.
     */
    static void reset(StandardModel *&model, const bool ask, QWidget *parent = nullptr) {

        if (ask) {
            // Ask for confirmation before resetting
            QMessageBox::StandardButton reply = QMessageBox::question(
                parent,
                "Confirm Reset",
                "Are you sure you want to reset the MIME types?",
                QMessageBox::Yes | QMessageBox::No
                );

            if (reply == QMessageBox::No)
                return;
        }

        const QStringList mimeTypesList = AppStrings::getString(StringType::MIMETYPES).split("\n", Qt::SkipEmptyParts);

        model->clearModel();
        model->appendNewItems(mimeTypesList, false, true);
    }


private:
    /**
     * Processes a string of MIME types, adding valid ones to the model and reporting invalid ones.
     * @param mimeTypesData - A string containing the MIME types to process.
     * @param mimetypesModel - Reference to the model where MIME types will be added.
     * @param parent - Optional parent widget for displaying error messages.
     */
    static void processMimeTypes(const QString &mimeTypesData, StandardModel *&mimetypesModel, QWidget *parent = nullptr) {

        if (mimeTypesData.isEmpty())
            return;

        // Split the text into MIME types, trimming spaces around semicolons
        QStringList mimeTypes = mimeTypesData.split(SEPARATOR_WITH_WHITESPACE_REGEX, Qt::SkipEmptyParts);

        QStringList unsupportedMimeTypes;
        QMimeDatabase mimeDatabase;

        for (QString &mimetype : mimeTypes) {
            mimetype = mimetype.trimmed();
            QMimeType qmimeType = mimeDatabase.mimeTypeForName(mimetype);

            if (qmimeType.isValid())
                mimetypesModel->appendNewItem(qmimeType.name(), false, true);
            else
                unsupportedMimeTypes << mimetype;
        }

        mimetypesModel->sort(2);

        // Show a message box if there are unsupported MIME types
        if (!unsupportedMimeTypes.isEmpty()) {
            ScrollableMessageBox msgBox("Invalid MIME Types",
                                        "The following MIME types aren't valid in Qt:\n",
                                        unsupportedMimeTypes, parent);
            msgBox.exec();
        }
    }

};
