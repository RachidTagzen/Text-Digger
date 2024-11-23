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

#include <QGuiApplication>
#include <QMimeData>
#include <QUrl>
#include <QStringList>
#include <QImage>
#include <QFileInfo>
#include <QClipboard>
#include <memory>  // For smart pointers
#include <QMutex>  // For thread safety

/**
 * @class Clipboard_Utils
 * @brief Utility class for clipboard operations.
 *
 * This class provides static methods to copy and paste various types of data to and from the clipboard.
 */
class Clipboard_Utils {

public:

    // *******************************************************************************************************************
    // ****************************************************** Copy *******************************************************
    // *******************************************************************************************************************
    /**
     * @brief Copies plain text to the clipboard.
     * @param text The text to copy.
     */
    static void copyPlainTextToClipboard(const QString &text) {
        QGuiApplication::clipboard()->setText(text);
    }

    /**
     * @brief Copies a list of text items to the clipboard.
     * @param textList The list of text items to copy.
     */
    static void copyTextListToClipboard(const QStringList &textList) {
        copyPlainTextToClipboard(textList.join(QLatin1Char('\n')));
    }

    /**
     * @brief Copies an image to the clipboard.
     * @param image The image to copy.
     */
    static void copyImageToClipboard(const QImage &image) {
        QGuiApplication::clipboard()->setImage(image);
    }

    /**
     * @brief Copies a URL to the clipboard.
     * @param url The URL to copy.
     */
    static void copyURLToClipboard(const QUrl &url) {
        copyUrlsToClipboard({url});
    }

    /**
     * @brief Copies a list of file paths to the clipboard.
     * @param fileList The list of file paths to copy.
     */
    static void copyFilesToClipboard(const QList<QFileInfo> &fileList) {
        QList<QUrl> urls;
        urls.reserve(fileList.size());
        for (const QFileInfo &fileInfo : fileList)
            urls.append(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));

        copyUrlsToClipboard(urls);
    }


    /**
     * @brief Copies a list of URLs to the clipboard.
     * @param urls The list of URLs to copy.
     */
    static void copyUrlsToClipboard(const QList<QUrl> &urls) {
        QMutexLocker locker(&mutex); // Lock the mutex for thread safety
        QMimeData *mimeData = new QMimeData();  // Use raw pointer as Qt will handle it
        mimeData->setUrls(urls);
        QGuiApplication::clipboard()->setMimeData(mimeData);  // Clipboard takes ownership
    }



    // *******************************************************************************************************************
    // ****************************************************** Paste ******************************************************
    // *******************************************************************************************************************
    /**
     * @brief Pastes plain text from the clipboard.
     * @return The plain text from the clipboard.
     */
    static QString pasteClipboardToPlainText() {
        return getMimeDataContent([](const QMimeData *mimeData) {
            return mimeData->hasText() ? mimeData->text() : "";
        });
    }

    /**
     * @brief Pastes a list of text items from the clipboard.
     * @return The list of text items from the clipboard.
     */
    static QStringList pasteClipboardToTextList() {
        return getMimeDataContent([](const QMimeData *mimeData) {
            return mimeData->hasText() ? mimeData->text().split(QLatin1Char('\n')) : QStringList();
        });
    }

    /**
     * @brief Pastes an image from the clipboard.
     * @return The image from the clipboard.
     */
    static QImage pasteClipboardToImage() {
        return getMimeDataContent([](const QMimeData *mimeData) {
            return mimeData->hasImage() ? qvariant_cast<QImage>(mimeData->imageData()) : QImage();
        });
    }

    /**
     * @brief Pastes a URL from the clipboard.
     * @return The URL from the clipboard.
     */
    static QUrl pasteClipboardToURL() {
        return getMimeDataContent([](const QMimeData *mimeData) {
            if (mimeData->hasUrls()) {
                const QList<QUrl> urls = mimeData->urls();
                return !urls.isEmpty() ? urls.first() : QUrl();
            }

            return QUrl();
        });
    }


    /**
     * @brief Pastes a list of file paths from the clipboard and returns them as QStringList.
     * @return QStringList containing file paths from the clipboard.
     *
     * This method ensures that only valid files and directories are accepted.
     */
    static QStringList pasteClipboardToFilePaths() {
        return getMimeDataContent([](const QMimeData *mimeData) {
            QStringList filePaths;

            auto processFileInfo = [&](const QString &path) {
                QFileInfo fileInfo(path.trimmed());
                if ((fileInfo.isFile() || fileInfo.isDir()) && fileInfo.isReadable() && fileInfo.isWritable()) {
                    filePaths.append(fileInfo.absoluteFilePath());
                }
            };

            if (mimeData->hasUrls()) {
                for (const QUrl &url : mimeData->urls()) {
                    QString localFile = url.toLocalFile();
                    if (!localFile.isEmpty())
                        processFileInfo(localFile);
                }
            } else if (mimeData->hasText()) {
                for (const QString &filePath : mimeData->text().split('\n')) {
                    if (!filePath.trimmed().isEmpty())
                        processFileInfo(filePath);
                }
            }

            return filePaths; // Return QStringList
        });
    }

    /**
     * @brief Pastes a list of file paths from the clipboard and returns them as QList<QFileInfo>.
     * @return QList<QFileInfo> containing file information from the clipboard.
     *
     * This method ensures that only valid files and directories are accepted.
     */
    static QList<QFileInfo> pasteClipboardToFileInfoList() {
        return getMimeDataContent([](const QMimeData *mimeData) {
            QList<QFileInfo> fileList;

            auto processFileInfo = [&](const QString &path) {
                QFileInfo fileInfo(path.trimmed());
                if ((fileInfo.isFile() || fileInfo.isDir()) && fileInfo.isReadable() && fileInfo.isWritable()) {
                    fileList.append(fileInfo);
                }
            };

            if (mimeData->hasUrls()) {
                for (const QUrl &url : mimeData->urls()) {
                    QString localFile = url.toLocalFile();
                    if (!localFile.isEmpty())
                        processFileInfo(localFile);
                }
            } else if (mimeData->hasText()) {
                for (const QString &filePath : mimeData->text().split('\n')) {
                    if (!filePath.trimmed().isEmpty())
                        processFileInfo(filePath);
                }
            }

            return fileList; // Return QList<QFileInfo>
        });
    }


private:
    // Mutex for thread safety
    inline static QMutex mutex;


    // Helper template for accessing the clipboard and applying a function to the mime data
    template<typename Func>
    static auto getMimeDataContent(Func func) -> decltype(func(nullptr)) {
        QMutexLocker locker(&mutex); // Lock the mutex for thread safety
        const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData();
        return func(mimeData);
    }
};
