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


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "enumerators/enums.h"
#include "stores/store_setting.h"
#include "databases/database_settings.h"
#include "components/filterwidget.h"
#include "components/statusbarwidget.h"
#include "models/standardmodel.h"
#include "models/results_model.h"
#include "models/results_sortfilterproxymodel.h"
#include "operations/op_find_occurrences.h"

#include <QMainWindow>
#include <QTableView>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showSettings();
    void showStatistics();
    void aboutApplication();

    bool eventFilter(QObject *object, QEvent *event);
    bool toggleCheckState(QStandardItemModel *model, const QModelIndex &index, int column);
    void setConnections();
    void clearViews();
    void disableControls(bool value);

    void startSearch();
    void cancelSearch();
    void searchFinished();
    void searchCanceled();

    bool clearLists();

    void viewResults(const QModelIndex &modelIndex);


private slots:
    void quitApp();
    void closeEvent(QCloseEvent *event);

    void handleContextMenus();
    void selectRows(QTableView *tableView, CheckingType checkingType);

    const QString getSettingValue(const QVector<Store_Setting> &settingsList, const QString &key) ;
    void loadSettings_FromDatabase();
    void saveSettings_ToDatabase();

    void openFileOnExternalApplication(const QModelIndex &index);
    void openFilesOnExternalApplication(const SelectionType selectionType);
    void copyFilesPaths(const SelectionType &selectionType);
    void deleteFiles(const SelectionType &selectionType);
    void replaceContent(const SelectionType &selectionType);
    QStringList getFilePaths(const SelectionType selectionType);

    void initialiazeControls();
    void initialiazeControlsFromSettings();


private:
    Ui::MainWindow *ui;

    App_Settings *m_appSettings;
    Database_Settings *m_database_Settings;

    FilterWidget *m_filterWidget_FindText;
    FilterWidget *m_filterWidget_Filenames;
    StatusBarWidget *m_statusBarWidget;

    bool m_handlingResultsSelection = false;     // This avoid calling viewResults() twice when pressing Up/Down keys

    bool m_isSearching = false;
    QSet<QString> m_checkedDirectoriesToInclude;
    QSet<QString> m_checkedDirectoriesToExclude;
    QSet<QMimeType> m_checkedMimeTypes;
    QStringList m_filesList;
    StandardModel *m_includedDirectoriesModel;
    StandardModel *m_excludedDirectoriesModel;
    StandardModel *m_mimetypesModel;
    ResultsModel *m_resultsModel;
    ResultsSortFilterProxyModel *m_resultsSortFilterProxyModel;

    QRegularExpression m_searchTextPattern;
    QString m_targetFilenames;
    QVector<QRegularExpression> m_filenamesPatterns;
    bool m_dontMatchText;
    bool m_dontMatchfilenames;

    QString m_lastOpenedIncludeDir = "";
    QString m_lastOpenedExcludeDir = "";
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

    FindOccurrences *m_findOccurrencesWorker;
    QThread *m_findOccurrencesThread;

    QElapsedTimer m_elapsedTimer;
    QString m_statsStartTime = "";
    QString m_statsEndTime = "";
    QString m_statsElapsed = "";
    QMap<QString, qint64> m_statisticsMap;

};



#endif // MAINWINDOW_H
