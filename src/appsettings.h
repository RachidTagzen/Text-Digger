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


#ifndef APPSETTINGS_H
#define APPSETTINGS_H


#include "constants/constants.h"
#include "stores/store_setting.h"

#include <QSize>


class App_Settings {


public:
    // *******************************************************************************************************************
    // ************************************************** Constructors ***************************************************
    // *******************************************************************************************************************
    explicit App_Settings() :
        m_filenames(""),
        m_filenamesCaseSensitive(false),
        m_dontMatchfilenames(false),
        m_filenamesPatternSyntax("Fixed String"),
        m_enableFindExactFilename(false),
        m_parseSubdirectories(true),
        m_minDepth(0),
        m_maxDepth(999),
        m_ignoreHiddenDirectories(false),
        m_ignoreSymbolicDirectoriesLinks(true),
        m_lastOpenedIncludeDir(""),
        m_directoriesToInclude(""),
        m_excludeDirectories(true),
        m_lastOpenedExcludeDir(""),
        m_directoriesToExclude(""),
        m_textToFind(""),
        m_textToFindCaseSensitive(false),
        m_dontMatchText(false),
        m_textToFindPatternSyntax("Fixed String"),
        m_matchWholeWords(false),
        m_filterBySize(false),
        m_sizeSystems(""),
        m_sizeConditions(""),
        m_fileSizeUnits_1(""),
        m_fileSizeUnits_2(""),
        m_fileSize_1(0),
        m_fileSize_2(0),
        m_filterByCreationDate(false),
        m_creationDateConditions(""),
        m_creationDate_1(""),
        m_creationDate_2(""),
        m_filterByLastModificationDate(false),
        m_lastModificationDateConditions(""),
        m_lastModificationDate_1(""),
        m_lastModificationDate_2(""),
        m_filterByLastAccessDate(false),
        m_lastAccessDateConditions(""),
        m_lastAccessDate_1(""),
        m_lastAccessDate_2(""),
        m_ignoreUnparseableFiles(true),
        m_ignoreHiddenFiles(false),
        m_ignoreSymbolicFilesLinks(true),
        m_avoidDuplicateFiles(false),
        m_enableFileReadingTimeout(true),
        m_fileReadingTimeout(60),
        m_enableFilesToParseLimit(true),
        m_filesToParseLimit(200),
        m_enableOccurrencesFoundLimit(false),
        m_occurrencesFoundLimit(300),
        m_filterByMimeTypes(false),
        m_mimeTypesList(""),
        m_rememberSize(true),
        m_windowSize("1075;700"),
        m_alwaysOnTop(false),
        m_enableLoggers(false),
        m_loggersFilesToKeep(100),
        m_lastResultsDirectory("")
    { }



    // *******************************************************************************************************************
    // ***************************************************** Getters *****************************************************
    // *******************************************************************************************************************
    inline QString getFilenames() const {
        return m_filenames;
    }

    inline bool isFilenamesCaseSensitive() const {
        return m_filenamesCaseSensitive;
    }

    inline bool isDontMatchfilenames() const {
        return m_dontMatchfilenames;
    }

    inline QString getFilenamesPatternSyntax() const {
        return m_filenamesPatternSyntax;
    }

    inline bool enableFindExactFilename() const {
        return m_enableFindExactFilename;
    }

    inline bool parseSubdirectories() const {
        return m_parseSubdirectories;
    }

    inline int getMinDepth() const {
        return m_minDepth;
    }

    inline int getMaxDepth() const {
        return m_maxDepth;
    }

    inline bool ignoreHiddenDirectories() const {
        return m_ignoreHiddenDirectories;
    }

    inline bool ignoreSymbolicDirectoriesLinks() const {
        return m_ignoreSymbolicDirectoriesLinks;
    }

    inline QString getLastOpenedIncludeDir() const {
        return m_lastOpenedIncludeDir;
    }

    inline QString getDirectoriesToInclude() const {
        return m_directoriesToInclude;
    }

    inline bool excludeDirectories() const {
        return m_excludeDirectories;
    }

    inline QString getLastOpenedExcludeDir() const {
        return m_lastOpenedExcludeDir;
    }

    inline QString getDirectoriesToExclude() const {
        return m_directoriesToExclude;
    }

    inline QString getTextToFind() const {
        return m_textToFind;
    }

    inline bool isTextToFindCaseSensitive() const {
        return m_textToFindCaseSensitive;
    }

    inline bool isDontMatchText() const {
        return m_dontMatchText;
    }

    inline QString getTextToFindPatternSyntax() const {
        return m_textToFindPatternSyntax;
    }

    inline bool matchWholeWords() const {
        return m_matchWholeWords;
    }

    inline bool filterBySize() const {
        return m_filterBySize;
    }

    inline QString getSizeSystems() const {
        return m_sizeSystems;
    }

    inline QString getSizeConditions() const {
        return m_sizeConditions;
    }

    inline QString getFileSizeUnits_1() const {
        return m_fileSizeUnits_1;
    }

    inline QString getFileSizeUnits_2() const {
        return m_fileSizeUnits_2;
    }

    inline int getFileSize_1() const {
        return m_fileSize_1;
    }

    inline int getFileSize_2() const {
        return m_fileSize_2;
    }

    inline bool filterByCreationDate() const {
        return m_filterByCreationDate;
    }

    inline QString getCreationDateConditions() const {
        return m_creationDateConditions;
    }

    inline QString getCreationDate_1() const {
        return m_creationDate_1;
    }

    inline QString getCreationDate_2() const {
        return m_creationDate_2;
    }

    inline bool filterByLastModificationDate() const {
        return m_filterByLastModificationDate;
    }

    inline QString getLastModificationDateConditions() const {
        return m_lastModificationDateConditions;
    }

    inline QString getLastModificationDate_1() const {
        return m_lastModificationDate_1;
    }

    inline QString getLastModificationDate_2() const {
        return m_lastModificationDate_2;
    }

    inline bool filterByLastAccessDate() const {
        return m_filterByLastAccessDate;
    }

    inline QString getLastAccessDateConditions() const {
        return m_lastAccessDateConditions;
    }

    inline QString getLastAccessDate_1() const {
        return m_lastAccessDate_1;
    }

    inline QString getLastAccessDate_2() const {
        return m_lastAccessDate_2;
    }

    inline bool ignoreUnparseableFiles() const {
        return m_ignoreUnparseableFiles;
    }

    inline bool ignoreHiddenFiles() const {
        return m_ignoreHiddenFiles;
    }

    inline bool ignoreSymbolicFilesLinks() const {
        return m_ignoreSymbolicFilesLinks;
    }

    inline bool avoidDuplicateFiles() const {
        return m_avoidDuplicateFiles;
    }

    inline bool enableFileReadingTimeout() const {
        return m_enableFileReadingTimeout;
    }

    inline int getFileReadingTimeout() const {
        return m_fileReadingTimeout;
    }

    inline bool enableFilesToParseLimit() const {
        return m_enableFilesToParseLimit;
    }

    inline int getFilesToParseLimit() const {
        return m_filesToParseLimit;
    }

    inline bool enableOccurrencesFoundLimit() const {
        return m_enableOccurrencesFoundLimit;
    }

    inline int getOccurrencesFoundLimit() const {
        return m_occurrencesFoundLimit;
    }

    inline bool filterByMimeTypes() const {
        return m_filterByMimeTypes;
    }

    inline QString getMimeTypesList() const {
        return m_mimeTypesList;
    }

    inline bool rememberSize() const {
        return m_rememberSize;
    }

    inline QSize getWindowSize() const {
        QStringList parts = m_windowSize.split(';');

        if (parts.size() != 2)
            return QSize(1075, 700);

        bool ok1, ok2;
        int width = parts[0].toInt(&ok1);
        int height = parts[1].toInt(&ok2);
        if (ok1 && ok2)
            return QSize(width, height);
         else
            return QSize(1075, 700);
    }

    inline bool alwaysOnTop() const {
        return m_alwaysOnTop;
    }

    inline bool enableLoggers() const {
        return m_enableLoggers;
    }

    inline int getLoggersFilesToKeep() const {
        return m_loggersFilesToKeep;
    }

    inline QString getLastResultsDirectory() const {
        return m_lastResultsDirectory;
    }



    // *******************************************************************************************************************
    // ***************************************************** Setters *****************************************************
    // *******************************************************************************************************************
    inline void setFilenames(const QString &newFilenames) {
        m_filenames = newFilenames;
    }

    inline void setFilenamesCaseSensitive(const bool &newFilenamesCaseSensitive) {
        m_filenamesCaseSensitive = newFilenamesCaseSensitive;
    }

    inline void setDontMatchfilenames(const bool &newDontMatchfilenames) {
        m_dontMatchfilenames = newDontMatchfilenames;
    }

    inline void setFilenamesPatternSyntax(const QString &newFilenamesPatternSyntax) {
        m_filenamesPatternSyntax = newFilenamesPatternSyntax;
    }

    inline void setEnableFindExactFilename(const bool &newEnableFindExactFilename) {
        m_enableFindExactFilename = newEnableFindExactFilename;
    }

    inline void setParseSubdirectories(const bool &newParseSubdirectories) {
        m_parseSubdirectories = newParseSubdirectories;
    }

    inline void setMinDepth(const int &newMinDepth) {
        m_minDepth = newMinDepth;
    }

    inline void setMaxDepth(const int &newMaxDepth) {
        m_maxDepth = newMaxDepth;
    }

    inline void setIgnoreHiddenDirectories(const bool &newIgnoreHiddenDirectories) {
        m_ignoreHiddenDirectories = newIgnoreHiddenDirectories;
    }

    inline void setIgnoreSymbolicDirectoriesLinks(const bool &newIgnoreSymbolicDirectoriesLinks) {
        m_ignoreSymbolicDirectoriesLinks = newIgnoreSymbolicDirectoriesLinks;
    }

    inline void setLastOpenedIncludeDir(const QString &newLastOpenedIncludeDir) {
        m_lastOpenedIncludeDir = newLastOpenedIncludeDir;
    }

    inline void setDirectoriesToInclude(const QMap<QString, bool> &directoriesToIncludeMap) {
        QStringList lines;

        for (auto it = directoriesToIncludeMap.constBegin(); it != directoriesToIncludeMap.constEnd(); ++it) {
            QString dirPath = it.key();
            QString status = it.value() ? "[1]" : "[0]";
            lines.append(status + " " + dirPath);
        }

        m_directoriesToInclude = lines.join("\n");
    }

    inline void setDirectoriesToInclude(const QString &directoriesToInclude) {
        m_directoriesToInclude = directoriesToInclude;
    }

    inline void setExcludeDirectories(const bool &newExcludeDirectories) {
        m_excludeDirectories = newExcludeDirectories;
    }

    inline void setLastOpenedExcludeDir(const QString &newLastOpenedExcludeDir) {
        m_lastOpenedExcludeDir = newLastOpenedExcludeDir;
    }

    inline void setDirectoriesToExclude(const QMap<QString, bool> &directoriesToExcludeMap) {
        QStringList lines;

        for (auto it = directoriesToExcludeMap.constBegin(); it != directoriesToExcludeMap.constEnd(); ++it) {
            QString dirPath = it.key();
            QString status = it.value() ? "[1]" : "[0]";
            lines.append(status + " " + dirPath);
        }

        m_directoriesToExclude = lines.join("\n");
    }

    inline void setDirectoriesToExclude(const QString &directoriesToExclude) {
        m_directoriesToExclude = directoriesToExclude;
    }

    inline void setTextToFind(const QString &newTextToFind) {
        m_textToFind = newTextToFind;
    }

    inline void setTextToFindCaseSensitive(const bool &newTextToFindCaseSensitive) {
        m_textToFindCaseSensitive = newTextToFindCaseSensitive;
    }

    inline void setDontMatchText(const bool &newDontMatchText) {
        m_dontMatchText = newDontMatchText;
    }

    inline void setTextToFindPatternSyntax(const QString &newTextToFindPatternSyntax) {
        m_textToFindPatternSyntax = newTextToFindPatternSyntax;
    }

    inline void setMatchWholeWords(const bool &newMatchWholeWords) {
        m_matchWholeWords = newMatchWholeWords;
    }

    inline void setFilterBySize(const bool &newFilterBySize) {
        m_filterBySize = newFilterBySize;
    }

    inline void setSizeSystems(const QString &newSizeSystems) {
        m_sizeSystems = newSizeSystems;
    }

    inline void setSizeConditions(const QString &newSizeConditions) {
        m_sizeConditions = newSizeConditions;
    }

    inline void setFileSizeUnits_1(const QString &newFileSizeUnits_1) {
        m_fileSizeUnits_1 = newFileSizeUnits_1;
    }

    inline void setFileSizeUnits_2(const QString &newFileSizeUnits_2) {
        m_fileSizeUnits_2 = newFileSizeUnits_2;
    }

    inline void setFileSize_1(const QString &newFileSize_1) {
        bool ok;
        int fileSize = newFileSize_1.toInt(&ok);
        m_fileSize_1 = ok ? fileSize : 0;
    }

    inline void setFileSize_2(const QString &newFileSize_2) {
        bool ok;
        int fileSize = newFileSize_2.toInt(&ok);
        m_fileSize_2 = ok ? fileSize : 0;
    }

    inline void setFilterByCreationDate(const bool &newFilterByCreationDate) {
        m_filterByCreationDate = newFilterByCreationDate;
    }

    inline void setCreationDateConditions(const QString &newCreationDateConditions) {
        m_creationDateConditions = newCreationDateConditions;
    }

    inline void setCreationDate_1(const QString &newCreationDate_1) {
        m_creationDate_1 = newCreationDate_1;
    }

    inline void setCreationDate_1(const QDateTime &newCreationDate_1) {
        m_creationDate_1 = newCreationDate_1.toString("yyyy-MM-dd hh:mm:ss");
    }

    inline void setCreationDate_2(const QString &newCreationDate_2) {
        m_creationDate_2 = newCreationDate_2;
    }

    inline void setCreationDate_2(const QDateTime &newCreationDate_2) {
        m_creationDate_2 = newCreationDate_2.toString("yyyy-MM-dd hh:mm:ss");
    }

    inline void setFilterByLastModificationDate(const bool &newFilterByLastModificationDate) {
        m_filterByLastModificationDate = newFilterByLastModificationDate;
    }

    inline void setLastModificationDateConditions(const QString &newLastModificationDateConditions) {
        m_lastModificationDateConditions = newLastModificationDateConditions;
    }

    inline void setLastModificationDate_1(const QString &newLastModificationDate_1) {
        m_lastModificationDate_1 = newLastModificationDate_1;
    }

    inline void setLastModificationDate_1(const QDateTime &newLastModificationDate_1) {
        m_lastModificationDate_1 = newLastModificationDate_1.toString("yyyy-MM-dd hh:mm:ss");
    }

    inline void setLastModificationDate_2(const QString &newLastModificationDate_2) {
        m_lastModificationDate_2 = newLastModificationDate_2;
    }

    inline void setLastModificationDate_2(const QDateTime &newLastModificationDate_2) {
        m_lastModificationDate_2 = newLastModificationDate_2.toString("yyyy-MM-dd hh:mm:ss");
    }

    inline void setFilterByLastAccessDate(const bool &newFilterByLastAccessDate) {
        m_filterByLastAccessDate = newFilterByLastAccessDate;
    }

    inline void setLastAccessDateConditions(const QString &newLastAccessDateConditions) {
        m_lastAccessDateConditions = newLastAccessDateConditions;
    }

    inline void setLastAccessDate_1(const QString &newLastAccessDate_1) {
        m_lastAccessDate_1 = newLastAccessDate_1;
    }

    inline void setLastAccessDate_1(const QDateTime &newLastAccessDate_1) {
        m_lastAccessDate_1 = newLastAccessDate_1.toString("yyyy-MM-dd hh:mm:ss");
    }

    inline void setLastAccessDate_2(const QString &newLastAccessDate_2) {
        m_lastAccessDate_2 = newLastAccessDate_2;
    }

    inline void setLastAccessDate_2(const QDateTime &newLastAccessDate_2) {
        m_lastAccessDate_2 = newLastAccessDate_2.toString("yyyy-MM-dd hh:mm:ss");
    }

    inline void setIgnoreUnparseableFiles(const bool &newIgnoreUnparseableFiles) {
        m_ignoreUnparseableFiles = newIgnoreUnparseableFiles;
    }

    inline void setIgnoreHiddenFiles(const bool &newIgnoreHiddenFiles) {
        m_ignoreHiddenFiles = newIgnoreHiddenFiles;
    }

    inline void setIgnoreSymbolicFilesLinks(const bool &newIgnoreSymbolicFilesLinks) {
        m_ignoreSymbolicFilesLinks = newIgnoreSymbolicFilesLinks;
    }

    inline void setAvoidDuplicateFiles(const bool &newAvoidDuplicateFiles) {
        m_avoidDuplicateFiles = newAvoidDuplicateFiles;
    }

    inline void setEnableFileReadingTimeout(const bool &newEnableFileReadingTimeout) {
        m_enableFileReadingTimeout = newEnableFileReadingTimeout;
    }

    inline void setFileReadingTimeout(const int &newFileReadingTimeout) {
        m_fileReadingTimeout = newFileReadingTimeout;
    }

    inline void setEnableFilesToParseLimit(const bool &newEnableFilesToParseLimit) {
        m_enableFilesToParseLimit = newEnableFilesToParseLimit;
    }

    inline void setFilesToParseLimit(const int &newFilesToParseLimit) {
        m_filesToParseLimit = newFilesToParseLimit;
    }

    inline void setEnableOccurrencesFoundLimit(const bool &newEnableOccurrencesFoundLimit) {
        m_enableOccurrencesFoundLimit = newEnableOccurrencesFoundLimit;
    }

    inline void setOccurrencesFoundLimit(const int &newOccurrencesFoundLimit) {
        m_occurrencesFoundLimit = newOccurrencesFoundLimit;
    }

    inline void setFilterByMimeTypes(const bool &newFilterByMimeTypes) {
        m_filterByMimeTypes = newFilterByMimeTypes;
    }

    inline void setMimeTypesList(const QMap<QString, bool> &mimeTypesMap) {

        QStringList lines;

        for (auto it = mimeTypesMap.constBegin(); it != mimeTypesMap.constEnd(); ++it) {
            QString mimeType = it.key();
            QString status = it.value() ? "[1]" : "[0]";
            lines.append(status + " " + mimeType);
        }

        m_mimeTypesList = lines.join("\n");
    }

    inline void setMimeTypesList(const QString &newMimeTypesList) {
        m_mimeTypesList = newMimeTypesList;
    }

    inline void setRememberSize(const bool &newRememberSize) {
        m_rememberSize = newRememberSize;
    }

    inline void setWindowSize(const QString &newWindowSize) {
        m_windowSize = newWindowSize;
    }

    inline void setWindowSize(const QSize &newWindowSize) {
        m_windowSize = QString("%1;%2").arg(newWindowSize.width()).arg(newWindowSize.height());
    }

    inline void setAlwaysOnTop(const bool &newAlwaysOnTop) {
        m_alwaysOnTop = newAlwaysOnTop;
    }

    inline void setEnableLoggers(const bool &newEnableLoggers) {
        m_enableLoggers = newEnableLoggers;
    }

    inline void setLoggersFilesToKeep(const int &newLoggersFilesToKeep) {
        m_loggersFilesToKeep = newLoggersFilesToKeep;
    }

    inline void setLastResultsDirectory(const QString &lastResultsDirectory) {
        m_lastResultsDirectory = lastResultsDirectory;
    }



    // *******************************************************************************************************************
    // **************************************************** Functions ****************************************************
    // *******************************************************************************************************************
    QVector<Store_Setting> getSettingsList() {

        settingsList.append(Store_Setting("m_textToFind",
                                          m_textToFind,
                                          ""));

        settingsList.append(Store_Setting("m_filenamesCaseSensitive",
                                          QString::number(m_filenamesCaseSensitive),
                                          QString::number(0)));

        settingsList.append(Store_Setting("m_dontMatchfilenames",
                                          QString::number(m_dontMatchfilenames),
                                          QString::number(0)));

        settingsList.append(Store_Setting("m_filenamesPatternSyntax",
                                          m_filenamesPatternSyntax,
                                          "Fixed String"));

        settingsList.append(Store_Setting("m_matchWholeWords",
                                          QString::number(m_matchWholeWords),
                                          QString::number(0)));


        settingsList.append(Store_Setting("m_parseSubdirectories",
                                          QString::number(m_parseSubdirectories),
                                          QString::number(1)));

        settingsList.append(Store_Setting("m_minDepth",
                                          QString::number(m_minDepth),
                                          QString::number(0)));

        settingsList.append(Store_Setting("m_maxDepth",
                                          QString::number(m_maxDepth),
                                          QString::number(999)));

        settingsList.append(Store_Setting("m_ignoreHiddenDirectories",
                                          QString::number(m_ignoreHiddenDirectories),
                                          QString::number(0)));

        settingsList.append(Store_Setting("m_ignoreSymbolicDirectoriesLinks",
                                          QString::number(m_ignoreSymbolicDirectoriesLinks),
                                          QString::number(1)));

        settingsList.append(Store_Setting("m_lastOpenedExcludeDir",
                                          m_lastOpenedExcludeDir,
                                          ""));

        settingsList.append(Store_Setting("m_directoriesToInclude",
                                          m_directoriesToInclude,
                                          ""));

        settingsList.append(Store_Setting("m_lastOpenedIncludeDir",
                                          m_lastOpenedIncludeDir,
                                          ""));

        settingsList.append(Store_Setting("m_excludeDirectories",
                                          QString::number(m_excludeDirectories),
                                          QString::number(1)));

        settingsList.append(Store_Setting("m_directoriesToExclude",
                                          m_directoriesToExclude,
                                          ""));

        settingsList.append(Store_Setting("m_filenames",
                                          m_filenames,
                                          ""));

        settingsList.append(Store_Setting("m_textToFindCaseSensitive",
                                          QString::number(m_textToFindCaseSensitive),
                                          QString::number(0)));

        settingsList.append(Store_Setting("m_dontMatchText",
                                          QString::number(m_dontMatchText),
                                          QString::number(0)));

        settingsList.append(Store_Setting("m_textToFindPatternSyntax",
                                          m_textToFindPatternSyntax,
                                          "Fixed String"));

        settingsList.append(Store_Setting("m_enableFindExactFilename",
                                          QString::number(m_enableFindExactFilename),
                                          QString::number(0)));

        settingsList.append(Store_Setting("m_filterBySize",
                                          QString::number(m_filterBySize),
                                          QString::number(0)));

        settingsList.append(Store_Setting("m_sizeSystems",
                                          m_sizeSystems,
                                          ""));

        settingsList.append(Store_Setting("m_sizeConditions",
                                          m_sizeConditions,
                                          ""));

        settingsList.append(Store_Setting("m_fileSizeUnits_1",
                                          m_fileSizeUnits_1,
                                          ""));

        settingsList.append(Store_Setting("m_fileSizeUnits_2",
                                          m_fileSizeUnits_2,
                                          ""));

        settingsList.append(Store_Setting("m_fileSize_1",
                                          QString::number(m_fileSize_1),
                                          QString::number(0)));

        settingsList.append(Store_Setting("m_fileSize_2",
                                          QString::number(m_fileSize_2),
                                          QString::number(0)));

        settingsList.append(Store_Setting("m_filterByCreationDate",
                                          QString::number(m_filterByCreationDate),
                                          QString::number(0)));

        settingsList.append(Store_Setting("m_creationDateConditions",
                                          m_creationDateConditions,
                                          ""));

        settingsList.append(Store_Setting("m_creationDate_1",
                                          m_creationDate_1,
                                          ""));

        settingsList.append(Store_Setting("m_creationDate_2",
                                          m_creationDate_2,
                                          ""));

        settingsList.append(Store_Setting("m_filterByLastModificationDate",
                                          QString::number(m_filterByLastModificationDate),
                                          QString::number(0)));

        settingsList.append(Store_Setting("m_lastModificationDateConditions",
                                          m_lastModificationDateConditions,
                                          ""));

        settingsList.append(Store_Setting("m_lastModificationDate_1",
                                          m_lastModificationDate_1,
                                          ""));

        settingsList.append(Store_Setting("m_lastModificationDate_2",
                                          m_lastModificationDate_2,
                                          ""));

        settingsList.append(Store_Setting("m_filterByLastAccessDate",
                                          QString::number(m_filterByLastAccessDate),
                                          QString::number(0)));

        settingsList.append(Store_Setting("m_lastAccessDateConditions",
                                          m_lastAccessDateConditions,
                                          ""));

        settingsList.append(Store_Setting("m_lastAccessDate_1",
                                          m_lastAccessDate_1,
                                          ""));

        settingsList.append(Store_Setting("m_lastAccessDate_2",
                                          m_lastAccessDate_2,
                                          ""));

        settingsList.append(Store_Setting("m_ignoreUnparseableFiles",
                                          QString::number(m_ignoreUnparseableFiles),
                                          QString::number(1)));

        settingsList.append(Store_Setting("m_ignoreHiddenFiles",
                                          QString::number(m_ignoreHiddenFiles),
                                          QString::number(0)));

        settingsList.append(Store_Setting("m_ignoreSymbolicFilesLinks",
                                          QString::number(m_ignoreSymbolicFilesLinks),
                                          QString::number(1)));

        settingsList.append(Store_Setting("m_avoidDuplicateFiles",
                                          QString::number(m_avoidDuplicateFiles),
                                          QString::number(0)));

        settingsList.append(Store_Setting("m_enableFileReadingTimeout",
                                          QString::number(m_enableFileReadingTimeout),
                                          QString::number(1)));

        settingsList.append(Store_Setting("m_fileReadingTimeout",
                                          QString::number(m_fileReadingTimeout),
                                          QString::number(60)));

        settingsList.append(Store_Setting("m_enableFilesToParseLimit",
                                          QString::number(m_enableFilesToParseLimit),
                                          QString::number(1)));

        settingsList.append(Store_Setting("m_filesToParseLimit",
                                          QString::number(m_filesToParseLimit),
                                          QString::number(200)));

        settingsList.append(Store_Setting("m_enableOccurrencesFoundLimit",
                                          QString::number(m_enableOccurrencesFoundLimit),
                                          QString::number(0)));

        settingsList.append(Store_Setting("m_occurrencesFoundLimit",
                                          QString::number(m_occurrencesFoundLimit),
                                          QString::number(300)));

        settingsList.append(Store_Setting("m_filterByMimeTypes",
                                          QString::number(m_filterByMimeTypes),
                                          QString::number(0)));

        settingsList.append(Store_Setting("m_mimeTypesList",
                                          m_mimeTypesList,
                                          ""));

        settingsList.append(Store_Setting("m_rememberSize",
                                          QString::number(m_rememberSize),
                                          QString::number(1)));

        settingsList.append(Store_Setting("m_windowSize",
                                          m_windowSize,
                                          "1075;700"));

        settingsList.append(Store_Setting("m_alwaysOnTop",
                                          QString::number(m_alwaysOnTop),
                                          QString::number(0)));

        settingsList.append(Store_Setting("m_enableLoggers",
                                          QString::number(m_enableLoggers),
                                          QString::number(0)));

        settingsList.append(Store_Setting("m_LoggersFilesToKeep",
                                          QString::number(m_loggersFilesToKeep),
                                          QString::number(100)));

        settingsList.append(Store_Setting("m_lastResultsDirectory",
                                          m_lastResultsDirectory,
                                          HOME_DIRECTORY.absolutePath()));

        return settingsList;
    }



private:
    QVector<Store_Setting> settingsList;

    QString m_filenames;
    bool m_filenamesCaseSensitive;
    bool m_dontMatchfilenames;
    QString m_filenamesPatternSyntax;
    bool m_enableFindExactFilename;

    bool m_parseSubdirectories;
    int m_minDepth;
    int m_maxDepth;
    bool m_ignoreHiddenDirectories;
    bool m_ignoreSymbolicDirectoriesLinks;

    QString m_lastOpenedIncludeDir;
    QString m_directoriesToInclude;

    bool m_excludeDirectories;
    QString m_lastOpenedExcludeDir;
    QString m_directoriesToExclude;

    QString m_textToFind;
    bool m_textToFindCaseSensitive;
    bool m_dontMatchText;
    QString m_textToFindPatternSyntax;
    bool m_matchWholeWords;

    bool m_filterBySize;
    QString m_sizeSystems;
    QString m_sizeConditions;
    QString m_fileSizeUnits_1;
    QString m_fileSizeUnits_2;
    int m_fileSize_1;
    int m_fileSize_2;

    bool m_filterByCreationDate;
    QString m_creationDateConditions;
    QString m_creationDate_1;
    QString m_creationDate_2;

    bool m_filterByLastModificationDate;
    QString m_lastModificationDateConditions;
    QString m_lastModificationDate_1;
    QString m_lastModificationDate_2;

    bool m_filterByLastAccessDate;
    QString m_lastAccessDateConditions;
    QString m_lastAccessDate_1;
    QString m_lastAccessDate_2;

    bool m_ignoreUnparseableFiles;
    bool m_ignoreHiddenFiles;
    bool m_ignoreSymbolicFilesLinks;
    bool m_avoidDuplicateFiles;

    bool m_enableFileReadingTimeout;
    int m_fileReadingTimeout;

    bool m_enableFilesToParseLimit;
    int m_filesToParseLimit;

    bool m_enableOccurrencesFoundLimit;
    int m_occurrencesFoundLimit;

    bool m_filterByMimeTypes;
    QString m_mimeTypesList;

    bool m_rememberSize;
    QString m_windowSize;
    bool m_alwaysOnTop;

    bool m_enableLoggers = false;
    int m_loggersFilesToKeep = 100;

    QString m_lastResultsDirectory;

};



#endif // APPSETTINGS_H
