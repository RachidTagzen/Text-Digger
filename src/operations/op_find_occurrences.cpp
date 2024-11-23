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


#include "operations/op_find_occurrences.h"

#include <QTimer>
#include <QThread>
#include <QMimeType>
#include <QDirIterator>

#include "constants/constants.h"
#include "utils/file_utils.h"
#include "utils/datetime_utils.h"
#include "hash/checksum_utils.h"


// *******************************************************************************************************************
// ************************************************** Constructors ***************************************************
// *******************************************************************************************************************
FindOccurrences::FindOccurrences(QSet<QString> &directories, QSet<QString> &excludeDirs, QSet<QMimeType> &mimetypes,
                                 QStringList &filesList, ResultsModel *&resultsModel, QRegularExpression &searchTextPattern,
                                 QString &targetFilenames, QVector<QRegularExpression> &filenamesPatterns,
                                 FilterWidget::PatternSyntax patternSyntax_Filenames,
                                 Qt::CaseSensitivity filenamesCaseSensitivity, bool matchText, bool dontMatchfilenames,
                                 bool subdirectories, int minDepth, int maxDepth, bool ignoreHiddenDirectories,
                                 bool ignoreHiddenFiles, bool ignoreSymbolicDirectoriesLinks,
                                 bool ignoreSymbolicFilesLinks, bool findExactFilename, bool ignoreUnparseableFiles,
                                 bool avoidDuplicates, QSet<QString> &filesHashes_Set, QDir::Filters filtersDirectories,
                                 QDir::Filters filtersFiles, QString &sizeCondition, QString &sizeSystem,
                                 double size_1, double size_2, QString &sizeUnits_1, QString &sizeUnits_2,
                                 QString &creationDateCondition, QDateTime &creationDate_1, QDateTime &creationDate_2,
                                 QString &lastModificationCondition, QDateTime &lastModificationDate_1,
                                 QDateTime &lastModificationDate_2, QString &lastAccessDateCondition,
                                 QDateTime &lastAccessDate_1, QDateTime &lastAccessDate_2, bool filterBySize,
                                 bool filterByCreationDate, bool filterByLastModificationDate,
                                 bool filterByLastAccessDate, bool filterByMimeTypes, bool fileReadingTimeout,
                                 bool limitFilesToParse, bool limitOccurrencesFound, int timeoutFileReading,
                                 int filesToParseLimit, int occurrencesFoundLimit, QObject *parent)

    : QObject(parent),
    m_cancel(false),
    m_directoriesToInclude(directories),
    m_directoriesToExclude(excludeDirs),
    m_mimetypes(mimetypes),
    m_filesList(filesList),
    m_resultsModel(resultsModel),
    m_searchTextPattern(searchTextPattern),
    m_targetFilenames(targetFilenames),
    m_filenamesPatterns(filenamesPatterns),
    m_patternSyntax_Filenames(patternSyntax_Filenames),
    m_filenamesCaseSensitivity(filenamesCaseSensitivity),
    m_matchText(matchText),
    m_dontMatchfilenames(dontMatchfilenames),
    m_subdirectories(subdirectories),
    m_minDepth(minDepth),
    m_maxDepth(maxDepth),
    m_ignoreHiddenDirectories(ignoreHiddenDirectories),
    m_ignoreHiddenFiles(ignoreHiddenFiles),
    m_ignoreSymbolicDirectoriesLinks(ignoreSymbolicDirectoriesLinks),
    m_ignoreSymbolicFilesLinks(ignoreSymbolicFilesLinks),
    m_findExactFilename(findExactFilename),
    m_ignoreUnparseableFiles(ignoreUnparseableFiles),
    m_avoidDuplicates(avoidDuplicates),
    m_filesHashes_Set(filesHashes_Set),
    m_filtersDirectories(filtersDirectories),
    m_filtersFiles(filtersFiles),
    m_sizeCondition(sizeCondition),
    m_sizeSystem(sizeSystem),
    m_size_1(size_1),
    m_size_2(size_2),
    m_sizeUnits_1(sizeUnits_1),
    m_sizeUnits_2(sizeUnits_2),
    m_creationDateCondition(creationDateCondition),
    m_creationDate_1(creationDate_1),
    m_creationDate_2(creationDate_2),
    m_lastModificationCondition(lastModificationCondition),
    m_lastModificationDate_1(lastModificationDate_1),
    m_lastModificationDate_2(lastModificationDate_2),
    m_lastAccessDateCondition(lastAccessDateCondition),
    m_lastAccessDate_1(lastAccessDate_1),
    m_lastAccessDate_2(lastAccessDate_2),
    m_filterBySize(filterBySize),
    m_filterByCreationDate(filterByCreationDate),
    m_filterByLastModificationDate(filterByLastModificationDate),
    m_filterByLastAccessDate(filterByLastAccessDate),
    m_filterByMimeTypes(filterByMimeTypes),
    m_fileReadingTimeout(fileReadingTimeout),
    m_limitFilesToParse(limitFilesToParse),
    m_limitOccurrencesFound(limitOccurrencesFound),
    m_timeoutFileReading(timeoutFileReading),
    m_filesToParseLimit(filesToParseLimit),
    m_occurrencesFoundLimit(occurrencesFoundLimit) { }



// *********************************************************************************************************************
// *********************************************************************************************************************
// *********************************************************************************************************************
void FindOccurrences::start() {
    
    qDebug() << "Searching operation started...";
    
    // --------------------------
    // Parse directories for files
    // --------------------------
    parseDirectories();
    
    if (m_cancel) {
        QStringList().swap(m_filesList);
        setStatistics();
        emit canceled(m_statisticsMap);
        return;
    }
    
    std::sort(m_filesList.begin(), m_filesList.end());
    
    // for (const QString &file : m_filesList)
    //     qDebug() << "Found File : " << file;
    
    
    // --------------------------
    // Filter & Finding inside files
    // --------------------------
    filterFiles();
    
    // for (const QString& file : m_filesList)
    //     qDebug() << "Filtered File : " << file;
    
    if (m_cancel) {
        QStringList().swap(m_filesList);
        setStatistics();
        emit canceled(m_statisticsMap);
        return;
    }
    

    setStatistics();
    emit finished(m_statisticsMap);
}


void FindOccurrences::cancel() {
    m_cancel = true;
}


// *******************************************************************************************************************
// ************************************************ Parse Directories ************************************************
// *******************************************************************************************************************
void FindOccurrences::parseDirectories() {
    
    emit updateStatusBarOperation("Parsing directories : ");
    
    // --------------------------
    // Remove directories from a the initial list if they have a parent or ancestor directory that is
    // already present in the list. This avoids redundant scanning of subdirectories multiple times.
    // --------------------------
    if (m_subdirectories)
        excludeSubdirectoriesWithParents();
    
    // --------------------------
    //
    // --------------------------
    m_filtersDirectories = QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable;
    m_filtersFiles = QDir::Files | QDir::NoDotAndDotDot | QDir::Readable;
    
    if (!m_ignoreHiddenDirectories)
        m_filtersDirectories |= QDir::Hidden;
    
    if (!m_ignoreHiddenFiles)
        m_filtersFiles |= QDir::Hidden;
    
    if (m_ignoreSymbolicDirectoriesLinks)
        m_filtersDirectories |= QDir::NoSymLinks;
    
    if (m_ignoreSymbolicFilesLinks)
        m_filtersFiles |= QDir::NoSymLinks;
    
    
    // --------------------------
    //
    // --------------------------
    for (auto it = m_directoriesToInclude.constBegin(); it != m_directoriesToInclude.constEnd(); ++it) {
        if (m_cancel)
            return;

        const QString &dirPath = *it;
        parseDirectory(dirPath, 0, m_statsProcessedFiles);
    }

}


void FindOccurrences::parseDirectory(const QString &dirPath, const int currentDepth, qint64 &filesParsedCount) {
    
    if (m_cancel)
        return;
    
    
    // If the limit is reached, return early
    if (m_limitFilesToParse && filesParsedCount >= m_filesToParseLimit) {
        qInfo() << "File limit reached. Stopping further parsing.";
        return;
    }
    
    if (currentDepth > m_maxDepth && m_maxDepth != -1)
        return;
    
    QFileInfo dirInfo(dirPath);
    if (!dirInfo.isDir() || !dirInfo.isReadable()) {
        qWarning() << "Directory is not readable or does not exist : " << dirPath;
        return;
    }
    
    emit updateStatusBarMessage(dirPath);
    
    // Collect files in the current directory. If subdirectories are disabled, always collect files.
    // If subdirectories are enabled, only collect files when currentDepth >= minDepth.
    if (!m_subdirectories || currentDepth >= m_minDepth) {
        // Collect files in the current directory.
        QDirIterator it(dirPath, m_filtersFiles, QDirIterator::NoIteratorFlags);
        while (it.hasNext() && (!m_limitFilesToParse || filesParsedCount < m_filesToParseLimit)) {
            const QString filePath = it.next();
            m_filesList << filePath;
            ++filesParsedCount;
        }
        
        m_statsProcessedDirectories++;
    }
    
    
    // Now parse subdirectories (recursively if needed)
    if (m_subdirectories) {
        QDirIterator subdirIt(dirPath, m_filtersDirectories, QDirIterator::NoIteratorFlags);
        while (subdirIt.hasNext() && (!m_limitFilesToParse || filesParsedCount < m_filesToParseLimit)) {
            QString subDirPath = subdirIt.next();
            QString absoluteSubDirPath = QFileInfo(subDirPath).absoluteFilePath();
            
            // Check if the subdirectory is in the exclude list            
            bool exclude = false;
            for (auto it = m_directoriesToExclude.constBegin(); it != m_directoriesToExclude.constEnd(); ++it) {
                const QString &excludeDir = *it;
                if (absoluteSubDirPath.startsWith(excludeDir)) {
                    exclude = true;
                    break;
                }
            }

            
            if (!exclude) {
                // Recurse into the subdirectory if not excluded
                parseDirectory(absoluteSubDirPath, currentDepth + 1, filesParsedCount);
                
                // Check again if the limit is reached after recursion
                if (m_limitFilesToParse && filesParsedCount >= m_filesToParseLimit) {
                    qInfo() << "File limit reached after parsing subdirectories. Stopping further parsing.";
                    return;
                }
            }
        }
    }
}


void FindOccurrences::excludeSubdirectoriesWithParents() {
    
    QStringList sortedDirs = QStringList(m_directoriesToInclude.begin(), m_directoriesToInclude.end());
    sortedDirs.sort();
    
    QSet<QString> cleanedSet;
    for (const QString &dir : sortedDirs) {
        if (std::none_of(cleanedSet.begin(), cleanedSet.end(),
                         [&dir](const QString &parentDir) { return dir.startsWith(parentDir + '/'); })) {
            cleanedSet.insert(dir);
        }
    }
    
    m_directoriesToInclude = cleanedSet;
}


// *******************************************************************************************************************
// ************************************************** Filter Files ***************************************************
// *******************************************************************************************************************
void FindOccurrences::filterFiles() {
    
    emit updateStatusBarOperation("Searching Occurrences : ");
    
    const QMimeDatabase mimeDatabase;
    
    for (const QString &filePath : m_filesList) {
        
        if (m_cancel)
            return;
        
        QFileInfo fileInfo(filePath);
        
        if (!matchFilenames(fileInfo.fileName()))
            continue;
        
        if (m_ignoreHiddenFiles && fileInfo.isHidden())
            continue;
        
        
        if (m_filterBySize)
            if (!Size_Utils::matchesSizeConditions(fileInfo.size(),
                                                   m_sizeSystem,
                                                   m_sizeCondition,
                                                   m_size_1,
                                                   m_size_2,
                                                   m_sizeUnits_1,
                                                   m_sizeUnits_2))
                continue;

        if (m_filterByCreationDate)
            if (!DateTime_Utils::matchesDateConditions(fileInfo.birthTime(),
                                                       m_creationDateCondition,
                                                       m_creationDate_1,
                                                       m_creationDate_2))
                continue;

        if (m_filterByLastModificationDate)
            if (!DateTime_Utils::matchesDateConditions(fileInfo.lastModified(),
                                                       m_lastModificationCondition,
                                                       m_lastModificationDate_1,
                                                       m_lastModificationDate_2))
                continue;

        if (m_filterByLastAccessDate)
            if (!DateTime_Utils::matchesDateConditions(fileInfo.lastRead(),
                                                       m_lastAccessDateCondition,
                                                       m_lastAccessDate_1,
                                                       m_lastAccessDate_2))
                continue;
        
        
        const QMimeType mimeType = mimeDatabase.mimeTypeForFile(fileInfo);
        
        if (m_filterByMimeTypes && !m_mimetypes.contains(mimeType))
            continue;
        
        
        parsingFiles(fileInfo, filePath, mimeType.name());
    }
    
    // Clear the Set and free up the memory
    QStringList().swap(m_filesList);
    
}


// *******************************************************************************************************************
// ************************************************** Parsing Files **************************************************
// *******************************************************************************************************************
void FindOccurrences::parsingFiles(const QFileInfo &fileInfo, const QString &filePath, const QString &mimeType) {
    
    if (m_cancel)
        return;
    
    QFile file(filePath);
    
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file" << filePath << ": " << file.errorString();
        return;
    }
    
    
    // Skip unparseable files if needed
    if (m_ignoreUnparseableFiles && !File_Utils::isTextFile(file)) {
        file.close();  // Close the file early if not parseable
        return;
    }
    
    
    // Check for duplicates and hash the file if required
    if (m_avoidDuplicates) {
        QString hashValue = ChecksumUtils::calculateMurmurHash3(file, ChecksumUtils::MurmurHash3Type::MURMUR_X64_128, false);
        if (!m_filesHashes_Set.contains(hashValue)) {
            m_filesHashes_Set.insert(hashValue);
        } else {
            file.close();  // Close the file early if it's a duplicate
            return;
        }
    }
    
    emit updateStatusBarMessage(filePath);



    const QPair<int, QSet<int>> occurencesFound = RescanOccurrences::scan(file,
                                                                          m_fileReadingTimeout,
                                                                          m_timeoutFileReading,
                                                                          m_limitOccurrencesFound,
                                                                          m_occurrencesFoundLimit,
                                                                          m_searchTextPattern,
                                                                          m_cancel);



    file.close();

    // Add the result to the results QVector if any occurrences were found
    bool shouldAppend = (m_matchText && occurencesFound.first > 0) || (!m_matchText && occurencesFound.first == 0);

    if (shouldAppend)
        m_resultsModel->appendNew(fileInfo,
                                  filePath,
                                  mimeType,
                                  m_sizeSystem,
                                  occurencesFound.first,
                                  occurencesFound.second,
                                  m_searchTextPattern,
                                  m_matchText);

}








// *********************************************************************************************************************
// *********************************************************************************************************************
// *********************************************************************************************************************
bool FindOccurrences::matchFilenames(const QString &filename) {
    
    if (m_targetFilenames.isEmpty())
        return true;
    
    // Determine the target match outcome based on m_dontMatchfilenames
    bool targetMatch = !m_dontMatchfilenames;
    
    // Check for exact filename match if required
    if (m_findExactFilename && m_patternSyntax_Filenames == FilterWidget::FixedString)
        return (filename.compare(m_targetFilenames, m_filenamesCaseSensitivity) == 0) == targetMatch;
    
    // Check patterns for match or non-match requirement
    for (const QRegularExpression &filenamePattern : m_filenamesPatterns)
        if (filenamePattern.match(filename).hasMatch())
            return targetMatch;  // Found a match, return based on the target condition
    
    // No matches found, return based on the opposite of the target condition
    return !targetMatch;
}


void FindOccurrences::setStatistics() {
    m_statisticsMap.insert("Processed Directories", m_statsProcessedDirectories);
    m_statisticsMap.insert("Processed Files", m_statsProcessedFiles);
}



