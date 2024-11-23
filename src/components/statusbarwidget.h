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

#include <QHBoxLayout>
#include <QLabel>


class StatusBarWidget : public QWidget {
    Q_OBJECT // Enables the use of signals and slots in this class


public:

    // *******************************************************************************************************************
    // ************************************************** Constructors ***************************************************
    // *******************************************************************************************************************
    /**
     * Constructor for the StatusBarWidget.
     * Initializes the status bar with default settings for the operation and message labels.
     * @param parent - Parent widget for this status bar (default is nullptr).
     */
    explicit StatusBarWidget(QWidget *parent = nullptr)
        : QWidget(parent), m_fontMetrics(m_message.font()) {

        // ------------------------
        // Configure operation label
        // ------------------------
        m_operation.setFixedHeight(16);
        m_operation.adjustSize();
        m_operation.setStyleSheet("font-weight: bold; color: red;");
        m_operation.setFocusPolicy(Qt::NoFocus);
        m_operation.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        // ------------------------
        // Configure message label
        // ------------------------
        m_message.setFixedHeight(16);
        m_message.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        m_message.setMinimumWidth(100);
        m_message.setStyleSheet("font-weight: bold; color: black;");
        m_message.setAlignment(Qt::AlignLeft | Qt::AlignVCenter);


        // ------------------------
        // Layout configuration
        // ------------------------
        m_layoutRoot.addWidget(&m_operation);
        m_layoutRoot.addWidget(&m_message);
        // m_layoutRoot.addStretch();


        // ------------------------
        // Set margins and layout
        // ------------------------
        setContentsMargins(0, 0, 0, 0);
        setLayout(&m_layoutRoot);


        // ------------------------
        // Signal connection
        // ------------------------
        connect(this, &StatusBarWidget::operationChanged, this, [this](const QString &text) {
            m_operation.setVisible(!text.trimmed().isEmpty());
        });

    }


    // *******************************************************************************************************************
    // ***************************************************** Setters *****************************************************
    // *******************************************************************************************************************
    /**
     * Sets the operation text and emits a signal that the operation has changed.
     * @param text - The text to set for the operation label.
     */
    void setOperation(const QString &text) {
        m_operation.setText(text);
        emit operationChanged(text);
    }


    /**
     * Sets the message text, eliding it if it exceeds the available width.
     * @param text - The text to set for the message label.
     */
    void setMessage(const QString &text) {
        // Dynamically calculate margin (if other elements exist in the status bar)
        const QMargins layoutMargins = m_layoutRoot.contentsMargins();
        int availableWidth = width() - layoutMargins.left() - layoutMargins.right() - m_operation.width() - 70;

        // Elide the text if it is too long to fit in the available width
        QString elidedText = m_fontMetrics.elidedText(text, Qt::ElideRight, availableWidth);
        m_message.setText(elidedText);
    }



    // *******************************************************************************************************************
    // **************************************************** Functions ****************************************************
    // *******************************************************************************************************************
    /**
     * Clears both the operation and message labels.
     */
    void clearAll() {
        clearOperation();
        clearMessage();
    }


    /**
     * Clears the operation text and emits a signal indicating it has changed.
     */
    void clearOperation() {
        if (!m_operation.text().isEmpty()) {
            m_operation.clear();
            emit operationChanged();
        }
    }


    /**
     * Clears the message text.
     */
    void clearMessage() {
        m_message.clear();
    }


signals:
    void operationChanged(const QString &text = "");


private:
    QHBoxLayout m_layoutRoot;
    QLabel m_operation;
    QLabel m_message;
    QFontMetrics m_fontMetrics;

};
