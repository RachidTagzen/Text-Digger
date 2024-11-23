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

#include "utils/clipboard_utils.h"

#include <QMessageBox>


class CopyFiles {

public:

    /**
     * Copies a list of file paths to the system clipboard.
     * If the list is empty, it shows a warning message to the user.
     *
     * @param filesPathsToCopy - A QStringList containing file paths to be copied to the clipboard.
     * @param parent - An optional QWidget pointer for setting the parent of dialog messages (default is nullptr).
     */
    static void copyFilesPaths(const QStringList &filesPathsToCopy, QWidget *parent = nullptr) {

        if (filesPathsToCopy.isEmpty()) {
            QMessageBox::warning(parent, "Warning", "No valid file paths found.");
            return;
        }

        Clipboard_Utils::copyTextListToClipboard(filesPathsToCopy);
        QMessageBox::information(parent, "Success", "File paths copied to clipboard.");
    }


};
