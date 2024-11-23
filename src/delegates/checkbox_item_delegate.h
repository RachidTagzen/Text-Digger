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

#include <QMouseEvent>
#include <QStyledItemDelegate>


class CheckBoxItemDelegate : public QStyledItemDelegate {

public:
    using QStyledItemDelegate::QStyledItemDelegate;

    /**
     * Custom paint method for rendering checkboxes, icons, and default content.
     * @param painter - The QPainter object for rendering.
     * @param option - Provides style options for the item.
     * @param index - The model index for the item to be painted.
     */
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override {

        // Initialize and style the item, drawing its background
        QStyleOptionViewItem opt = option;
        const QWidget *widget = option.widget;
        initStyleOption(&opt, index);
        QStyle *style = opt.widget ? opt.widget->style() : QApplication::style();
        style->drawPrimitive(QStyle::PE_PanelItemViewItem, &opt, painter, widget);

        // Handle checkboxes if the item has a check indicator
        if (opt.features & QStyleOptionViewItem::HasCheckIndicator) {
            // Set checkbox state based on the model's data
            switch (opt.checkState) {
            case Qt::Unchecked:
                opt.state |= QStyle::State_Off;
                break;
            case Qt::PartiallyChecked:
                opt.state |= QStyle::State_NoChange;
                break;
            case Qt::Checked:
                opt.state |= QStyle::State_On;
                break;
            }

            // Get the position for the checkbox and draw it
            auto rect = style->subElementRect(QStyle::SE_ItemViewItemCheckIndicator, &opt, widget);
            opt.rect = QStyle::alignedRect(opt.direction,
                                           index.data(Qt::TextAlignmentRole).value<Qt::Alignment>(),
                                           rect.size(),
                                           opt.rect);

            opt.state = opt.state & ~QStyle::State_HasFocus;
            style->drawPrimitive(QStyle::PE_IndicatorItemViewItemCheck, &opt, painter, widget);


        } else if (!opt.icon.isNull()) {	        // Handle icons if no checkbox is present
            // draw the icon
            QRect iconRect = style->subElementRect(QStyle::SE_ItemViewItemDecoration, &opt, widget);
            iconRect = QStyle::alignedRect(opt.direction,
                                           index.data(Qt::TextAlignmentRole).value<Qt::Alignment>(),
                                           iconRect.size(),
                                           opt.rect);

            QIcon::Mode mode = QIcon::Normal;

            if (!(opt.state & QStyle::State_Enabled))
                mode = QIcon::Disabled;
            else if (opt.state & QStyle::State_Selected)
                mode = QIcon::Selected;

            QIcon::State state = opt.state & QStyle::State_Open ? QIcon::On : QIcon::Off;
            opt.icon.paint(painter, iconRect, opt.decorationAlignment, mode, state);

        } else {	// Fallback to default paint behavior
            QStyledItemDelegate::paint(painter, option, index);
        }

    }


protected:
    /**
     * Handles user interactions with checkboxes (clicks, keypresses).
     * @param event - The event to handle.
     * @param model - The model that contains the data.
     * @param option - Style options for the item.
     * @param index - Model index of the item.
     * @return bool - True if the event was handled, false otherwise.
     */
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override {

        Q_ASSERT(event);
        Q_ASSERT(model);

        // Ensure the item is checkable and enabled
        Qt::ItemFlags flags = model->flags(index);

        if (!(flags & Qt::ItemIsUserCheckable) || !(option.state & QStyle::State_Enabled) || !(flags & Qt::ItemIsEnabled))
            return false;

        // Check if the item has a valid check state
        QVariant value = index.data(Qt::CheckStateRole);

        if (!value.isValid())
            return false;

        const QWidget *widget = option.widget;
        QStyle *style = option.widget ? widget->style() : QApplication::style();

        // Handle mouse events (clicks, double-clicks)
        if ((event->type() == QEvent::MouseButtonRelease) ||
            (event->type() == QEvent::MouseButtonDblClick) ||
            (event->type() == QEvent::MouseButtonPress)) {

            QStyleOptionViewItem viewOpt(option);
            initStyleOption(&viewOpt, index);

            QRect checkRect = style->subElementRect(QStyle::SE_ItemViewItemCheckIndicator, &viewOpt, widget);

            checkRect = QStyle::alignedRect(viewOpt.direction,
                                            index.data(Qt::TextAlignmentRole).value<Qt::Alignment>(),
                                            checkRect.size(),
                                            viewOpt.rect);

            QMouseEvent *me = static_cast<QMouseEvent *>(event);

            // Only handle left-button events within checkbox bounds
            if (me->button() != Qt::LeftButton || !checkRect.contains(me->pos()))
                return false;
            if ((event->type() == QEvent::MouseButtonPress) || (event->type() == QEvent::MouseButtonDblClick))
                return true;

            // Handle key events (space or select keys)
        } else if (event->type() == QEvent::KeyPress) {
            int key = static_cast<QKeyEvent *>(event)->key();
            if (key != Qt::Key_Space && key != Qt::Key_Select)
                return false;

        } else {
            return false;  // Ignore other events
        }

        // Toggle the checkbox state (supports tristate if enabled)
        Qt::CheckState state = static_cast<Qt::CheckState>(value.toInt());

        if (flags & Qt::ItemIsUserTristate)
            state = ((Qt::CheckState)((state + 1) % 3));
        else
            state = (state == Qt::Checked) ? Qt::Unchecked : Qt::Checked;

        return model->setData(index, state, Qt::CheckStateRole);
    }

};

