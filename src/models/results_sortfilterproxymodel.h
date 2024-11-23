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

#include <QSortFilterProxyModel>


class ResultsSortFilterProxyModel : public QSortFilterProxyModel {

public:
    ResultsSortFilterProxyModel(QObject *parent = nullptr) : QSortFilterProxyModel(parent) {}


protected:
    /**
     * Overrides the lessThan function to handle sorting based on column type.
     * @param left - The left index for comparison.
     * @param right - The right index for comparison.
     * @return bool - Returns true if the left index should appear before the right.
     */
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override {

        // Custom sorting for checkbox column (column index 2)
        if (left.column() == 2) {
            Qt::CheckState leftState = static_cast<Qt::CheckState>(sourceModel()->data(left, Qt::CheckStateRole).toInt());
            Qt::CheckState rightState = static_cast<Qt::CheckState>(sourceModel()->data(right, Qt::CheckStateRole).toInt());
            return leftState < rightState;

        // Custom sorting for the "Size" column (column index 5)
        } else if (left.column() == 5) {
            qint64 leftSize = sourceModel()->data(left, Qt::UserRole + 1).toLongLong();
            qint64 rightSize = sourceModel()->data(right, Qt::UserRole + 1).toLongLong();
            return leftSize < rightSize;

        // Custom sorting for date columns (Created, Modified, Accessed - columns 7, 8, 9)
        } else if (left.column() == 7 || left.column() == 8 || left.column() == 9) {
            QDateTime leftDate = sourceModel()->data(left, Qt::UserRole).toDateTime();
            QDateTime rightDate = sourceModel()->data(right, Qt::UserRole).toDateTime();
            return leftDate < rightDate;

        // Custom sorting for the "Occurrences" column (column index 10)
        } else if (left.column() == 10) {
            int leftOccurrences = sourceModel()->data(left, Qt::UserRole + 1).toInt();
            int rightOccurrences = sourceModel()->data(right, Qt::UserRole + 1).toInt();
            return leftOccurrences < rightOccurrences;
        }

        // Default sorting behavior for other columns
        return QSortFilterProxyModel::lessThan(left, right);
    }

};
