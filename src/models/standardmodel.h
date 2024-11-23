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

#include "hash/checksum_utils.h"

#include <QStandardItemModel>
#include <QUuid>


class StandardModel : public QStandardItemModel {
    Q_OBJECT


public:
    /**
     * Constructor to initialize the model with a specified content type and an optional parent.
     * Sets up the column count and headers based on the content type.
     * @param hashContentType - Specifies if the model will handle directory paths or MIME types.
     * @param parent - The parent QObject, typically the parent widget or object.
     */
    StandardModel(const HashContentType &hashContentType, QObject *parent = nullptr)
        : QStandardItemModel(parent), hashContentType(hashContentType) {

        setColumnCount(3);
        setHorizontalHeaderItem(0, new QStandardItem("Uuid"));
        setHorizontalHeaderItem(1, new QStandardItem("√"));

        // Set column header based on content type
        if (hashContentType == DirectoryPath)
            setHorizontalHeaderItem(2, new QStandardItem("Directory"));
        else if (hashContentType == MimeType)
            setHorizontalHeaderItem(2, new QStandardItem("MIME Type"));
    }


    /**
     * Adds a new item to the model if it doesn't already exist (based on hash).
     * @param content - The content (directory path or MIME type) to be added.
     * @param check - The default checked state of the item.
     * @param avoidDuplicateItem - If true, checks for duplicates before adding.
     */
    void appendNewItem(const QString &content, const bool check, const bool &avoidDuplicateItem) {

        // --------------------------
        // Generate a unique hash for the content to check for duplicates
        // --------------------------
        const QString contentHash = ChecksumUtils::generateMurmurHash(content, hashContentType);

        // --------------------------
        // Skip if duplicate check is enabled and content already exists
        // --------------------------
        if (avoidDuplicateItem && uniqueContents.contains(contentHash))
            return;

        // --------------------------
        // UUID item: unique identifier for the row
        // --------------------------
        QStandardItem *uuidItem = new QStandardItem(QUuid::createUuid().toString());
        uuidItem->setTextAlignment(Qt::AlignCenter);

        // --------------------------
        // Checkbox item for user selection, center-aligned
        // --------------------------
        QStandardItem *item_checkbox = new QStandardItem();
        item_checkbox->setFlags(item_checkbox->flags() | Qt::ItemIsUserCheckable);
        item_checkbox->setTextAlignment(Qt::AlignCenter);
        item_checkbox->setCheckable(true);
        item_checkbox->setEditable(false);
        item_checkbox->setCheckState(check ? Qt::Checked : Qt::Unchecked);

        // --------------------------
        // Content item (directory or MIME type), to be displayed in the third column
        // --------------------------
        QStandardItem *contentItem = new QStandardItem(content);

        // --------------------------
        // Append the new row with uuid, checkbox, and content items
        // --------------------------
        appendRow({uuidItem, item_checkbox, contentItem});

        // --------------------------
        // Track added content to prevent duplicates if needed
        // --------------------------
        uniqueContents.insert(contentHash);
    }


    /**
     * Adds multiple items to the model.
     * @param items - List of contents to be added.
     * @param check - The default checked state for the items.
     * @param avoidDuplicateItem - If true, prevents duplicates.
     */
    void appendNewItems(const QStringList &items, const bool check, const bool &avoidDuplicateItem) {
        for (const QString &content : items)
            appendNewItem(content, check, avoidDuplicateItem);
    }

    /**
     * Removes a specific row from the model, also removing it from the unique content set.
     * @param row - The row index to remove.
     */
    void removeItem(int row) {
        if (row >= 0 && row < rowCount()) {
            // Retrieve the content from the row and generate its hash
            QString content = data(index(row, 2)).toString();
            QString contentHash = ChecksumUtils::generateMurmurHash(content, hashContentType);
            uniqueContents.remove(contentHash);  // Remove from unique contents set
            removeRow(row);                      // Remove the row from the model
        }
    }

    /**
     * Removes multiple rows from the model in reverse order to prevent index shifting.
     * @param rows - List of row indices to remove.
     */
    void removeItems(const QList<int> &rows) {
        QList<int> sortedRows = rows;

        // Sort rows in reverse order to avoid shifting issues
        std::sort(sortedRows.begin(), sortedRows.end(), std::greater<int>());

        for (int row : sortedRows)
            removeItem(row);
    }


    /**
     * Check if the model is empty.
     */
    bool isEmpty() {
        return rowCount() == 0;
    }


    /**
     * Clears the model, removing all rows and resetting the unique content set.
     */
    void clearModel() {
        removeRows(0, rowCount());
        QSet<QString>().swap(uniqueContents);  // Clear the unique contents set
    }



    // signals:
    //     void checkboxStateChanged(bool isChecked); // Notify when checkbox state changes
    //     void anyItemChecked(bool hasChecked); // Notify if any item is checked


    // protected:
    // // Check if any items are checked
    // bool hasCheckedItems() const {
    //     for (int row = 0; row < rowCount(); ++row) {
    //         QStandardItem *itemInColumn = item(row, 1); // 2nd column
    //         if (itemInColumn && itemInColumn->checkState() == Qt::Checked) {
    //             return true; // Found a checked item
    //         }
    //     }
    //     return false; // No checked items
    // }

    // // Override setData to emit signals
    // bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override {
    //     if (role == Qt::CheckStateRole && index.column() == 1) {
    //         bool success = QStandardItemModel::setData(index, value, role);
    //         if (success) {
    //             emit checkboxStateChanged(value == Qt::Checked);
    //             emit anyItemChecked(hasCheckedItems());
    //         }
    //         return success;
    //     }
    //     return QStandardItemModel::setData(index, value, role);
    // }


private:
    HashContentType hashContentType;             // Stores the type of content this model manages
    QSet<QString> uniqueContents;        // Set to keep track of unique content hashes


};
