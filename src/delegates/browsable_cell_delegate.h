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

#include <QStyledItemDelegate>
#include <QPushButton>
#include <QPainter>
#include <QStyleOptionButton>
#include <QStyleOptionViewItem>
#include <QApplication>
#include <QMouseEvent>
#include <QDesktopServices>
#include <QUrl>
#include <QToolTip>
#include <QHelpEvent>

class BrowsableCellDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    explicit BrowsableCellDelegate(Qt::TextElideMode elideMode = Qt::ElideRight, QObject *parent = nullptr)
        : QStyledItemDelegate(parent), m_elideMode(elideMode) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {

        if (!index.isValid()) {
            QStyledItemDelegate::paint(painter, option, index);
            return;
        }

        // Check if this is one of our target cells (column 1, rows 4 or 5)
        bool isTargetCell = (index.column() == 1 && (index.row() == 4 || index.row() == 5));

        if (!isTargetCell) {
            // For non-target cells, use default painting
            QStyledItemDelegate::paint(painter, option, index);
            return;
        }

        // Custom painting for target cells only
        QString text = index.data(Qt::DisplayRole).toString();
        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);

        painter->save();

        // Setup rectangles for layout
        QRect textRect = opt.rect;
        const int buttonWidth = 60;
        const int buttonHeight = 30;
        const int margin = 2;

        // Calculate button rectangle
        QRect buttonRect = option.rect;
        buttonRect.setWidth(buttonWidth);
        buttonRect.setHeight(buttonHeight);
        buttonRect.moveLeft(option.rect.right() - buttonWidth - margin);
        buttonRect.moveTop(option.rect.top() + (option.rect.height() - buttonHeight) / 2);

        // Adjust text rectangle to avoid button
        textRect.setRight(buttonRect.left() - margin);

        // Draw background
        if (opt.state & QStyle::State_Selected)
            painter->fillRect(opt.rect, opt.palette.highlight());
        else
            painter->fillRect(opt.rect, opt.palette.base());

        // Draw text
        QFontMetrics fontMetrics(opt.font);
        QString elidedText = fontMetrics.elidedText(text, m_elideMode, textRect.width());
        painter->setPen(opt.palette.color(QPalette::Text));
        painter->drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft, elidedText);

        // Draw button
        QStyleOptionButton buttonOption;
        buttonOption.rect = buttonRect;
        buttonOption.text = "Browse";
        QApplication::style()->drawControl(QStyle::CE_PushButton, &buttonOption, painter);

        painter->restore();
    }



    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                     const QModelIndex &index) override {
        if (!index.isValid())
            return QStyledItemDelegate::editorEvent(event, model, option, index);

        bool showButton = (index.row() == 4 || index.row() == 5) && index.column() == 1;

        if (showButton && event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);

            // Calculate button rect
            const int buttonWidth = 65;
            const int buttonHeight = 30;
            const int margin = 5;

            QRect buttonRect = option.rect;
            buttonRect.setWidth(buttonWidth);
            buttonRect.setHeight(buttonHeight);
            buttonRect.moveLeft(option.rect.right() - buttonWidth - margin);
            buttonRect.moveTop(option.rect.top() + (option.rect.height() - buttonHeight) / 2);

            // Check if the mouse click is within the button
            if (buttonRect.contains(mouseEvent->pos())) {
                QString dirPath = index.data().toString(); // Get directory path from model
                if (!dirPath.isEmpty())
                    QDesktopServices::openUrl(QUrl::fromLocalFile(dirPath));

                return true;
            }
        }

        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }


private:
    Qt::TextElideMode m_elideMode;


};
