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

#include "models/results_model.h"
#include "components/statusbarwidget.h"
#include "components/filterwidget.h"

#include <QFileInfo>
#include <QMimeDatabase>


class FindOccurrences : public QObject {
    Q_OBJECT // If you're using Qt, you need this macro for signals and slots


public:
    FindOccurrences(QSet<QString> &directories, QSet<QString> &excludeDirs, QSet<QMimeType> &mimetypes,
                    QStringList &filesList, ResultsModel *&resultsModel, QRegularExpression &searchTextPattern,
                    QString &targetFilenames, QVector<QRegularExpression> &filenamesPatterns,
                    FilterWidget::PatternSyntax patternSyntax_Filenames, Qt::CaseSensitivity filenamesCaseSensitivity,
                    bool matchText, bool dontMatchfilenames, bool subdirectories, int minDepth, int maxDepth,
                    bool ignoreHiddenDirectories, bool ignoreHiddenFiles, bool ignoreSymbolicDirectoriesLinks,
                    bool ignoreSymbolicFilesLinks, bool findExactFilename, bool ignoreUnparseableFiles,
                    bool avoidDuplicates, QSet<QString> &filesHashes_Set, QDir::Filters filtersDirectories,
                    QDir::Filters filtersFiles, QString &sizeCondition, QString &sizeSystem, double size_1, double size_2,
                    QString &sizeUnits_1, QString &sizeUnits_2, QString &creationDateCondition, QDateTime &creationDate_1,
                    QDateTime &creationDate_2, QString &lastModificationCondition, QDateTime &lastModificationDate_1,
                    QDateTime &lastModificationDate_2, QString &lastAccessDateCondition, QDateTime &lastAccessDate_1,
                    QDateTime &lastAccessDate_2, bool filterBySize, bool filterByCreationDate,
                    bool filterByLastModificationDate, bool filterByLastAccessDate, bool filterByMimeTypes,
                    bool fileReadingTimeout, bool limitFilesToParse, bool limitOccurrencesFound,
                    int timeoutFileReading, int filesToParseLimit, int occurrencesFoundLimit, QObject *parent);

    void start();
    void cancel();
    void parseDirectories();
    void parseDirectory(const QString &dirPath, const int currentDepth, qint64 &filesParsedCount);
    void excludeSubdirectoriesWithParents();
    void filterFiles();
    void parsingFiles(const QFileInfo &fileInfo, const QString &filePath, const QString &mimeType);
    bool matchFilenames(const QString &filename);
    void setStatistics();


signals:

    // void sendLogger(QString status, QString url, QString destination, QColor color);
    void finished(const QMap<QString, qint64> &statisticsMap);
    void failed(const QMap<QString, qint64> &statisticsMap);
    void canceled(const QMap<QString, qint64> &statisticsMap);
    void updateStatusBarOperation(const QString &operation);
    void updateStatusBarMessage(const QString &message);


private:
    bool m_cancel;

    QSet<QString> m_directoriesToInclude;
    QSet<QString> m_directoriesToExclude;
    QSet<QMimeType> m_mimetypes;
    QStringList m_filesList;
    ResultsModel *m_resultsModel;

    QRegularExpression m_searchTextPattern;
    QString m_targetFilenames;
    QVector<QRegularExpression> m_filenamesPatterns;
    FilterWidget::PatternSyntax m_patternSyntax_Filenames;
    Qt::CaseSensitivity m_filenamesCaseSensitivity;
    bool m_matchText;
    bool m_dontMatchfilenames;

    bool m_subdirectories = false;
    int m_minDepth = 0;
    int m_maxDepth = 1;
    bool m_ignoreHiddenDirectories = false;
    bool m_ignoreHiddenFiles = false;
    bool m_ignoreSymbolicDirectoriesLinks = false;
    bool m_ignoreSymbolicFilesLinks = false;
    bool m_findExactFilename = false;
    bool m_ignoreUnparseableFiles = true;
    bool m_avoidDuplicates = false;

    QSet<QString> m_filesHashes_Set;
    QDir::Filters m_filtersDirectories = QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable;
    QDir::Filters m_filtersFiles = QDir::Files | QDir::NoDotAndDotDot | QDir::Readable;

    QString m_sizeCondition;
    QString m_sizeSystem;
    double m_size_1;
    double m_size_2;
    QString m_sizeUnits_1;
    QString m_sizeUnits_2;
    QString m_creationDateCondition;
    QDateTime m_creationDate_1;
    QDateTime m_creationDate_2;
    QString m_lastModificationCondition;
    QDateTime m_lastModificationDate_1;
    QDateTime m_lastModificationDate_2;
    QString m_lastAccessDateCondition;
    QDateTime m_lastAccessDate_1;
    QDateTime m_lastAccessDate_2;

    bool m_filterBySize = false;
    bool m_filterByCreationDate = false;
    bool m_filterByLastModificationDate = false;
    bool m_filterByLastAccessDate = false;
    bool m_filterByMimeTypes = false;

    bool m_fileReadingTimeout = false;
    bool m_limitFilesToParse = false;
    bool m_limitOccurrencesFound = false;
    int m_timeoutFileReading = 0;
    int m_filesToParseLimit = 0;
    int m_occurrencesFoundLimit = 0;

    qint64 m_statsProcessedDirectories = 0;
    qint64 m_statsProcessedFiles = 0;
    QMap<QString, qint64> m_statisticsMap;

};
