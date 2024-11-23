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
#include "constants/resources.h"

#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QSvgRenderer>
#include <QPainter>


class ScrollableMessageBox : public QDialog {
    Q_OBJECT // Enables the use of signals and slots in this class


public:
    /**
     * Constructor for the ScrollableMessageBox.
     * Initializes the dialog with a title, a static message, and a list of items to display.
     * @param title - The title of the dialog window.
     * @param staticMessage - A static message to display at the top.
     * @param items - A list of items to display in a scrollable area.
     * @param parent - Parent widget for this dialog (default is nullptr).
     */
    ScrollableMessageBox(const QString &title, const QString &staticMessage, const QStringList &items, QWidget *parent = nullptr)
        : QDialog(parent) {
        setupUI(title, staticMessage, items); // Call setupUI to initialize the dialog's user interface
    }


    /**
     * Sets up the user interface for the dialog.
     * @param title - The title of the dialog window.
     * @param staticMessage - A static message to display at the top.
     * @param items - A list of items to display in a scrollable area.
     */
    void setupUI(const QString &title, const QString &staticMessage, const QStringList &items) {
        setWindowTitle(title);

        // Create a QVBoxLayout for the main layout of the dialog
        QVBoxLayout *layout = new QVBoxLayout(this);

        // Create a QLabel for the static message (outside the scrollable area)
        QLabel *staticLabel = new QLabel(staticMessage, this);
        staticLabel->setWordWrap(true);  // Enable word wrapping for the static message

        // Add the static message label to the main layout (not scrollable)
        layout->addWidget(staticLabel);

        // Create a QTextEdit for the list of items (scrollable content)
        QTextEdit *scrollableTextEdit = new QTextEdit(this);
        scrollableTextEdit->setText(items.join("\n"));
        scrollableTextEdit->setReadOnly(true);
        scrollableTextEdit->setWordWrapMode(QTextOption::WrapAnywhere);  // Enable word wrapping for long lines

        // Apply a flat style to the QTextEdit
        scrollableTextEdit->setStyleSheet(AppStyles::getStyle(StyleType::GLOBAL));
        // Create a QScrollArea to hold the scrollable text edit
        QScrollArea *scrollArea = new QScrollArea(this);
        scrollArea->setWidget(scrollableTextEdit); // Set the QTextEdit as the widget for the scroll area
        scrollArea->setWidgetResizable(true);  // Ensure the text edit adjusts to the scroll area

        // Set the size policies for the scroll area and text edit to be resizable
        scrollArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        scrollableTextEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // Add the scroll area to the main layout
        layout->addWidget(scrollArea);

        // Create a horizontal layout for buttons
        QHBoxLayout *buttonLayout = new QHBoxLayout();

        // Create a button to copy text to clipboard
        QPushButton *copyButton = new QPushButton("Copy to Clipboard", this);
        setButtonIcon(copyButton, AppIcons::getIcon(IconType::COPY));
        buttonLayout->addWidget(copyButton);

        // Create an "OK" button
        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok, this);
        buttonLayout->addWidget(buttonBox);

        // Connect the button's clicked signal to the copy function
        connect(copyButton, &QPushButton::clicked, this, [this, scrollableTextEdit]() {
            Clipboard_Utils::copyPlainTextToClipboard(scrollableTextEdit->toPlainText());
        });

        // Connect the "OK" button to close the dialog
        connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);

        // Add the horizontal button layout to the main layout
        layout->addLayout(buttonLayout);

        // Set minimum size and resize the dialog dynamically
        setMinimumSize(400, 300);
        resize(600, 400);  // Set an initial size (adjust as needed)
    }


private:
    /**
     * Sets the icon for a button using an SVG file.
     * @param button - The button to set the icon for.
     * @param svgFilePath - The file path to the SVG icon.
     */
    void setButtonIcon(QPushButton *button, const QIcon &icon) {
        QSize iconSize(16, 16);
        button->setIcon(icon);
        button->setIconSize(iconSize);
    }

};
