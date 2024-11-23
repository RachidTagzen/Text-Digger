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


#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "settingswindow.h"
#include "statisticswindow.h"
#include "aboutwindow.h"
#include "enumerators/enums.h"
#include "hash/checksum_utils.h"
#include "constants/constants.h"
#include "constants/resources.h"
#include "utils/center_utils.h"
#include "utils/clipboard_utils.h"
#include "utils/datetime_utils.h"
#include "utils/directories_utils.h"
#include "utils/file_utils.h"
#include "utils/size_utils.h"
#include "utils/mimetypes_utils.h"
#include "delegates/checkbox_item_delegate.h"
#include "components/scrollable_messagebox.h"
#include "operations/op_copy_files.h"
#include "operations/op_delete_files.h"
#include "operations/op_handle_results.h"
#include "operations/op_open_files.h"
#include "operations/op_replace_ocurrences.h"

#include <QMessageBox>
#include <QFile>
#include <QStringList>
#include <QKeyEvent>
#include <QListWidgetItem>
#include <QDesktopServices>
#include <QFileDialog>
#include <QInputDialog>

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>


// *********************************************************************************************************************
// *********************************************************************************************************************
// *********************************************************************************************************************

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);


    // -----------------------------------------
    // Test zone
    // -----------------------------------------



    // -----------------------------------------
    //
    // -----------------------------------------
    CenterMainWindow(this);


    // -----------------------------------------
    // Create Directories & Files
    // -----------------------------------------
    File_Utils::createNecessaryDirectoriesAndFiles(false);


    // -----------------------------------------
    // Initilize variables
    // -----------------------------------------
    m_appSettings = new App_Settings();


    // -----------------------------------------
    // Apply Styles
    // -----------------------------------------
    qApp->setStyleSheet(AppStyles::getStyle(StyleType::GLOBAL)); // Apply globally



    // -----------------------------------------
    //  Models
    // -----------------------------------------
    m_resultsModel = new ResultsModel(this);
    m_resultsSortFilterProxyModel = new ResultsSortFilterProxyModel(this);
    m_resultsSortFilterProxyModel->setSourceModel(m_resultsModel);
    m_resultsSortFilterProxyModel->setSortRole(Qt::DisplayRole);
    ui->tableView_Results->setModel(m_resultsSortFilterProxyModel);

    m_includedDirectoriesModel = new StandardModel(DirectoryPath, this);
    ui->tableView_IncludeDirectories->setModel(m_includedDirectoriesModel);

    m_excludedDirectoriesModel = new StandardModel(DirectoryPath, this);
    ui->tableView_ExcludeDirectories->setModel(m_excludedDirectoriesModel);

    m_mimetypesModel = new StandardModel(MimeType, this);
    ui->tableView_MimeTypes->setModel(m_mimetypesModel);



    // -----------------------------------------
    // tableView_Result
    // -----------------------------------------
    ui->tableView_Results->setItemDelegateForColumn(1, new CheckBoxItemDelegate);
    ui->tableView_Results->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_Results->verticalHeader()->setDefaultSectionSize(22);
    ui->tableView_Results->horizontalHeader()->setFixedHeight(30);
    ui->tableView_Results->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableView_Results->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->tableView_Results->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->tableView_Results->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableView_Results->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->tableView_Results->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);
    ui->tableView_Results->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Fixed);
    ui->tableView_Results->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Fixed);
    ui->tableView_Results->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Fixed);
    ui->tableView_Results->horizontalHeader()->setSectionResizeMode(9, QHeaderView::Fixed);
    ui->tableView_Results->horizontalHeader()->setSectionResizeMode(10, QHeaderView::Fixed);
    ui->tableView_Results->setColumnWidth(1, 22);
    ui->tableView_Results->setColumnWidth(2, 22);
    ui->tableView_Results->setColumnWidth(5, 90);
    ui->tableView_Results->setColumnWidth(6, 220);
    ui->tableView_Results->setColumnWidth(7, 150);
    ui->tableView_Results->setColumnWidth(8, 150);
    ui->tableView_Results->setColumnWidth(9, 150);
    ui->tableView_Results->setColumnWidth(10, 80);
    ui->tableView_Results->setColumnHidden(0, true);
    ui->tableView_Results->setColumnHidden(7, true);
    ui->tableView_Results->setColumnHidden(9, true);
    ui->tableView_Results->setColumnHidden(11, true);


    // -----------------------------------------
    // tableView_IncludeDirectories
    // -----------------------------------------
    ui->tableView_IncludeDirectories->setItemDelegateForColumn(1, new CheckBoxItemDelegate);
    ui->tableView_IncludeDirectories->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_IncludeDirectories->verticalHeader()->setDefaultSectionSize(22);
    ui->tableView_IncludeDirectories->horizontalHeader()->setFixedHeight(30);
    ui->tableView_IncludeDirectories->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableView_IncludeDirectories->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_IncludeDirectories->setColumnHidden(0, true);
    ui->tableView_IncludeDirectories->setColumnWidth(1, 22);

    // -----------------------------------------
    // tableView_ExcludeDirectories
    // -----------------------------------------
    ui->tableView_ExcludeDirectories->setItemDelegateForColumn(1, new CheckBoxItemDelegate);
    ui->tableView_ExcludeDirectories->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_ExcludeDirectories->verticalHeader()->setDefaultSectionSize(22);
    ui->tableView_ExcludeDirectories->horizontalHeader()->setFixedHeight(30);
    ui->tableView_ExcludeDirectories->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableView_ExcludeDirectories->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_ExcludeDirectories->setColumnHidden(0, true);
    ui->tableView_ExcludeDirectories->setColumnWidth(1, 22);

    // -----------------------------------------
    // tableView_IncludeDirectories
    // -----------------------------------------
    ui->tableView_MimeTypes->setItemDelegateForColumn(1, new CheckBoxItemDelegate);
    ui->tableView_MimeTypes->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView_MimeTypes->verticalHeader()->hide();
    ui->tableView_MimeTypes->verticalHeader()->setDefaultSectionSize(22);
    ui->tableView_MimeTypes->horizontalHeader()->setFixedHeight(30);
    ui->tableView_MimeTypes->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableView_MimeTypes->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_MimeTypes->setColumnHidden(0, true);
    ui->tableView_MimeTypes->setColumnWidth(1, 22);



    // -----------------------------------------
    // Splitter
    // -----------------------------------------
    ui->splitter_result->setSizes({400, 400});


    // -----------------------------------------
    // Setup StatusBar
    // -----------------------------------------
    m_statusBarWidget = new StatusBarWidget(this);
    ui->statusbar->addPermanentWidget(m_statusBarWidget, 1);


    // -----------------------------------------
    //  Filter Widget Component
    // -----------------------------------------
    m_filterWidget_FindText = new FilterWidget;
    ui->horizontalLayout_FindText->insertWidget(1, m_filterWidget_FindText);
    m_filterWidget_FindText->setText("jar");
    m_filterWidget_FindText->setToolTip(AppStrings::getString(StringType::TOOLTIP_FINDTEXT));

    m_filterWidget_Filenames = new FilterWidget;
    ui->gridLayout_Filenames->addWidget(m_filterWidget_Filenames, 0, 1);
    m_filterWidget_Filenames->setToolTip(AppStrings::getString(StringType::TOOLTIP_FILENAMES));



    // -----------------------------------------
    // Load settings from database
    // -----------------------------------------
    m_database_Settings = new Database_Settings();
    m_database_Settings->createTables(m_appSettings); // If database file is just created
    initialiazeControls();


    // -----------------------------------------
    // Enable Keys detect
    // -----------------------------------------
    qApp->installEventFilter(this);
    // ui->tableView_Result->installEventFilter(this);


    // -----------------------------------------
    // Connections
    // -----------------------------------------
    setConnections();



    // -----------------------------------------
    // Test zone
    // -----------------------------------------


}


MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::quitApp() {
    QCoreApplication::quit();
}


void MainWindow::closeEvent(QCloseEvent *event) {
    // Task to do before closing window
    saveSettings_ToDatabase();
    event->accept();
}



// *********************************************************************************************************************
// *************************************************** Open Dialogs ****************************************************
// *********************************************************************************************************************
void MainWindow::showSettings() {
    SettingsWindow *settingsWindow = new SettingsWindow(m_appSettings, this);
    settingsWindow->setWindowModality(Qt::WindowModal);
    settingsWindow->setAttribute(Qt::WA_DeleteOnClose);  // Deletes `settingsWindow` on close
    CenterWidget(settingsWindow, this);

    settingsWindow->show();
}


void MainWindow::showStatistics() {

    // Allocate StatisticsModel with `statisticsWindow` as the parent
    StatisticsModel *statisticsModel = new StatisticsModel(this);
    statisticsModel->calculate(m_resultsModel, m_statsStartTime, m_statsEndTime, m_statsElapsed, m_statisticsMap);

    // Allocate StatisticsWindow with `statisticsModel` as its model
    StatisticsWindow *statisticsWindow = new StatisticsWindow(statisticsModel, this);
    statisticsWindow->setWindowModality(Qt::WindowModal);
    statisticsWindow->setAttribute(Qt::WA_DeleteOnClose);  // Deletes `statisticsWindow` on close
    CenterWidget(statisticsWindow, this);

    // Parent `statisticsModel` to `statisticsWindow` to ensure it's deleted with `statisticsWindow`
    statisticsModel->setParent(statisticsWindow);

    statisticsWindow->show();
}


void MainWindow::aboutApplication() {
    AboutWindow *aboutWindow = new AboutWindow(this);
    aboutWindow->setWindowModality(Qt::WindowModal);
    aboutWindow->setAttribute(Qt::WA_DeleteOnClose);
    CenterWidget(aboutWindow, this);  // Center relative to the mainwindow
    aboutWindow->show();
}


// *********************************************************************************************************************
// *************************************************** Event Filters ***************************************************
// *********************************************************************************************************************
bool MainWindow::eventFilter(QObject *object, QEvent *event) {

    if (!m_resultsModel)
        QWidget::eventFilter(object, event);


    if (event->type() == QEvent::KeyPress) {

        QKeyEvent *pKeyEvent = static_cast<QKeyEvent*>(event);

        if (object == ui->tableView_Results) {
            QKeyEvent* pKeyEvent = static_cast<QKeyEvent*>(event);

            if (ui->tableView_Results->hasFocus()) {

                QModelIndex currentIndex = ui->tableView_Results->currentIndex();

                if (pKeyEvent->key() == Qt::Key_Up || pKeyEvent->key() == Qt::Key_Down) {

                    if (m_handlingResultsSelection)
                        return true;

                    int currentRow = currentIndex.row();
                    currentRow += (pKeyEvent->key() == Qt::Key_Down ? 1 : -1);
                    QModelIndex targetIndex = ui->tableView_Results->model()->index(currentRow, currentIndex.column());

                    if (currentRow >= 0 && currentRow < m_resultsModel->rowCount()) {
                        m_handlingResultsSelection = true;  // Set flag before changing selection
                        ui->tableView_Results->setCurrentIndex(targetIndex);
                        viewResults(targetIndex);
                        m_handlingResultsSelection = false;  // Reset flag after handling
                    }

                } else if (pKeyEvent->key() == Qt::Key_PageUp || pKeyEvent->key() == Qt::Key_PageDown) {
                    int rowCount = ui->tableView_Results->model()->rowCount();
                    int currentRow = currentIndex.row();
                    int pageSize = ui->tableView_Results->height() / ui->tableView_Results->rowHeight(0);

                    int newRow = pKeyEvent->key() == Qt::Key_PageUp
                                     ? qMax(currentRow - pageSize, 0)
                                     : qMin(currentRow + pageSize, rowCount - 1);

                    ui->tableView_Results->selectRow(newRow);
                    ui->tableView_Results->scrollTo(ui->tableView_Results->model()->index(newRow, 0));

                } else if (pKeyEvent->key() == Qt::Key_Space) {
                    QModelIndex sourceIndex = m_resultsSortFilterProxyModel->mapToSource(currentIndex);
                    QStandardItem *item = m_resultsModel->item(sourceIndex.row(), 2);

                    if (item && item->isCheckable())
                        item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
                }

                event->accept();
                return true;
            }

        } else if (object == ui->tableView_IncludeDirectories) {
            if (pKeyEvent->key() == Qt::Key_Space) {
                QModelIndex currentIndex = ui->tableView_IncludeDirectories->currentIndex();
                if (toggleCheckState(m_includedDirectoriesModel, currentIndex, 1)) {
                    event->accept();
                    return true;
                }
            }

        } else if (object == ui->tableView_ExcludeDirectories) {
            if (pKeyEvent->key() == Qt::Key_Space) {
                QModelIndex currentIndex = ui->tableView_ExcludeDirectories->currentIndex();
                if (toggleCheckState(m_excludedDirectoriesModel, currentIndex, 1)) {
                    event->accept();
                    return true;
                }
            }

        } else if (object == ui->tableView_MimeTypes) {
            if (pKeyEvent->key() == Qt::Key_Space) {
                QModelIndex currentIndex = ui->tableView_MimeTypes->currentIndex();
                if (toggleCheckState(m_mimetypesModel, currentIndex, 1)) {
                    event->accept();
                    return true;
                }
            }

        } else if (m_filterWidget_FindText->hasFocus()) {
            if (pKeyEvent->key() == Qt::Key_Return || pKeyEvent->key() == Qt::Key_Enter) {
                startSearch();
                event->accept();
                return true;
            }
        }

    }

    // Let the base class handle the event
    return QWidget::eventFilter(object, event);
}


bool MainWindow::toggleCheckState(QStandardItemModel* model, const QModelIndex& index, int column) {
    if (!model || !index.isValid())
        return false;

    QStandardItem *item = model->item(index.row(), column);
    if (item && item->isCheckable()) {
        item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
        return true;
    }

    return false;
}



// *********************************************************************************************************************
// **************************************************** Connections ****************************************************
// *********************************************************************************************************************
void MainWindow::setConnections() {

    // -------------------------------
    // Subdirectories
    // -------------------------------
    connect(ui->checkBox_Subdirectories, &QCheckBox::toggled, this, [this](const bool checked) {
        ui->spinBox_MinDepth->setEnabled(checked);
        ui->spinBox_MaxDepth->setEnabled(checked);
    });


    // -------------------------------
    // Included Directories
    // -------------------------------
    connect(ui->btn_AddIncludedDirectory, &QPushButton::clicked, this, [this]() {
        Directories_Utils::addFromModel(m_includedDirectoriesModel, this);
    });

    connect(ui->btn_DeleteIncludedDirectories, &QPushButton::clicked, this, [this]() {
        QModelIndexList selectedIndexes = ui->tableView_IncludeDirectories->selectionModel()->selectedRows();
        Directories_Utils::removeFromModel(selectedIndexes, m_includedDirectoriesModel, this);
    });

    connect(ui->btn_PasteIncludedDirectories, &QPushButton::clicked, this, [this]() {
        QStringList directoriesList = Clipboard_Utils::pasteClipboardToFilePaths();
        m_includedDirectoriesModel->appendNewItems(directoriesList, false, true);
        m_includedDirectoriesModel->sort(2, Qt::AscendingOrder);
    });

    connect(ui->btn_CopyIncludedDirectories, &QPushButton::clicked, this, [this]() {
        QModelIndexList selectedIndexes = ui->tableView_IncludeDirectories->selectionModel()->selectedRows();
        Directories_Utils::copyToClipboard(selectedIndexes, m_includedDirectoriesModel);
    });

    // connect(m_includedDirectoriesModel, &StandardModel::anyItemChecked, [this](bool hasCheckedItem) {
    //     ui->btn_StartSearch->setEnabled(hasCheckedItem);
    // });


    // -------------------------------
    // Excluded Directories
    // -------------------------------
    connect(ui->btn_AddExcludedDirectory, &QPushButton::clicked, this, [this]() {
        Directories_Utils::addFromModel(m_excludedDirectoriesModel, this);
    });

    connect(ui->btn_DeleteExcludedDirectories, &QPushButton::clicked, this, [this]() {
        QModelIndexList selectedIndexes = ui->tableView_ExcludeDirectories->selectionModel()->selectedRows();
        Directories_Utils::removeFromModel(selectedIndexes, m_excludedDirectoriesModel, this);
    });

    connect(ui->btn_PasteExcludedDirectories, &QPushButton::clicked, this, [this]() {
        QStringList directoriesList = Clipboard_Utils::pasteClipboardToFilePaths();
        m_excludedDirectoriesModel->appendNewItems(directoriesList, false, true);
        m_excludedDirectoriesModel->sort(2, Qt::AscendingOrder);
    });

    connect(ui->btn_CopyExcludedDirectories, &QPushButton::clicked, this, [this]() {
        QModelIndexList selectedIndexes = ui->tableView_ExcludeDirectories->selectionModel()->selectedRows();
        Directories_Utils::copyToClipboard(selectedIndexes, m_excludedDirectoriesModel);
    });


    // -------------------------------
    // MIME Types
    // -------------------------------
    connect(ui->btn_AddMimeTypes, &QPushButton::clicked, this, [this]() {
        MimeTypes_Utils::addToModel(m_mimetypesModel, this);
    });

    connect(ui->btn_DeleteMimeTypes, &QPushButton::clicked, this, [this]() {
        QModelIndexList selectedIndexes = ui->tableView_MimeTypes->selectionModel()->selectedRows();
        MimeTypes_Utils::deleteSelected(selectedIndexes, m_mimetypesModel, this);
    });

    connect(ui->btn_PasteMimeTypes, &QPushButton::clicked, this, [this]() {
        MimeTypes_Utils::pasteFromClipboard(m_mimetypesModel, this);
    });

    connect(ui->btn_CopyMimeTypes, &QPushButton::clicked, this, [this]() {
        QModelIndexList selectedIndexes = ui->tableView_MimeTypes->selectionModel()->selectedRows();
        MimeTypes_Utils::copyToClipboard(selectedIndexes, m_mimetypesModel);
    });

    connect(ui->btn_ResetMimeTypes, &QPushButton::clicked, this, [this]() {
        MimeTypes_Utils::reset(m_mimetypesModel, true);
    });


    // -------------------------------
    // Size
    // -------------------------------
    connect(ui->checkBox_Size, &QCheckBox::toggled, this, [this](const bool checked) {
        ui->comboBox_SizeConditions->setEnabled(checked);
        ui->comboBox_SizeSystems->setEnabled(checked);
        ui->comboBox_SizeUnits_1->setEnabled(checked);
        ui->comboBox_SizeUnits_2->setEnabled(checked);
        ui->lineEdit_Size_1->setEnabled(checked);
        ui->lineEdit_Size_2->setEnabled(checked);
    });


    connect(ui->comboBox_SizeSystems, &QComboBox::currentTextChanged, this, [this](const QString &system) {
        ui->comboBox_SizeUnits_1->clear();
        ui->comboBox_SizeUnits_2->clear();

        if (system == "SI") {
            ui->comboBox_SizeUnits_1->addItems({"Byte", "Bytes", "KB", "MB", "GB", "TB"});
            ui->comboBox_SizeUnits_2->addItems({"Byte", "Bytes", "KB", "MB", "GB", "TB"});
        } else if (system == "IEC") {
            ui->comboBox_SizeUnits_1->addItems({"Byte", "Bytes", "KiB", "MiB", "GiB", "TiB"});
            ui->comboBox_SizeUnits_2->addItems({"Byte", "Bytes", "KiB", "MiB", "GiB", "TiB"});
        }
    });


    connect(ui->comboBox_SizeConditions, &QComboBox::currentTextChanged, this, [this](const QString &condition) {
        const bool visible = condition == "Between" || condition == "Not between";
        ui->lineEdit_Size_2->setVisible(visible);
        ui->comboBox_SizeUnits_2->setVisible(visible);
    });


    // -------------------------------
    // Creation Date
    // -------------------------------
    connect(ui->checkBox_CreationDate, &QCheckBox::toggled, this, [this](const bool checked) {
        ui->comboBox_CreationDateConditions->setEnabled(checked);
        ui->dateTimeEdit_CreationDate_1->setEnabled(checked);
        ui->dateTimeEdit_CreationDate_2->setEnabled(checked);
    });

    connect(ui->comboBox_CreationDateConditions, &QComboBox::currentTextChanged, this, [this](const QString &condition) {
        const bool visible = condition == "Between" || condition == "Not between";
        ui->dateTimeEdit_CreationDate_2->setVisible(visible);
    });


    // -------------------------------
    // Last Modification Date
    // -------------------------------
    connect(ui->checkBox_LastModificationDate, &QCheckBox::toggled, this, [this](const bool checked) {
        ui->comboBox_LastModificationDateConditions->setEnabled(checked);
        ui->dateTimeEdit_LastModificationDate_1->setEnabled(checked);
        ui->dateTimeEdit_LastModificationDate_2->setEnabled(checked);
    });

    connect(ui->comboBox_LastModificationDateConditions, &QComboBox::currentTextChanged, this, [this](const QString &condition) {
        const bool visible = condition == "Between" || condition == "Not between";
        ui->dateTimeEdit_LastModificationDate_2->setVisible(visible);
    });

    // -------------------------------
    // Last Access Date
    // -------------------------------
    connect(ui->checkBox_LastAccessDate, &QCheckBox::toggled, this, [this](const bool checked) {
        ui->comboBox_LastAccessDateConditions->setEnabled(checked);
        ui->dateTimeEdit_LastAccessDate_1->setEnabled(checked);
        ui->dateTimeEdit_LastAccessDate_2->setEnabled(checked);
    });

    connect(ui->comboBox_LastAccessDateConditions, &QComboBox::currentTextChanged, this, [this](const QString &condition) {
        const bool visible = condition == "Between" || condition == "Not between";
        ui->dateTimeEdit_LastAccessDate_2->setVisible(visible);
    });


    // -------------------------------
    // Results TableView
    // -------------------------------
    connect(ui->tableView_Results, &QTableView::doubleClicked, this, &MainWindow::openFileOnExternalApplication);

    connect(ui->tableView_Results->selectionModel(), &QItemSelectionModel::selectionChanged, this, [this]() {

        // Only process if not handling key navigation
        if (m_handlingResultsSelection)
            return;

        const QItemSelectionModel *selectionModel = ui->tableView_Results->selectionModel();

        ui->textEdit_View->setDisabled(!selectionModel->hasSelection());
        ui->textEdit_ViewHeader->setDisabled(!selectionModel->hasSelection());

        if (!selectionModel->hasSelection())
            return;

        QModelIndex selectedIndex = selectionModel->currentIndex();
        viewResults(selectedIndex);
    });


    // -------------------------------
    // Miscellaneous
    // -------------------------------
    connect(m_filterWidget_FindText, &FilterWidget::textChanged, this, [this](const QString &text) {
        ui->btn_StartSearch->setDisabled(text.trimmed().isEmpty());
    });

    connect(ui->btn_StartSearch, &QPushButton::clicked, this, &MainWindow::startSearch);

    connect(m_filterWidget_Filenames, &FilterWidget::disableExactMatchCheckbox,
            ui->checkBox_FindExactFilename, &QCheckBox::setDisabled);

    connect(ui->checkBox_FindExactFilename, &QCheckBox::toggled,
            m_filterWidget_Filenames, &FilterWidget::disablePatternsActions);

    connect(ui->checkBox_FileReadingTimeout, &QCheckBox::toggled, this, [this](const bool checked) {
        ui->spinBox_FileReadingTimeout->setEnabled(checked);
    });

    connect(ui->checkBox_FilesToParse, &QCheckBox::toggled, this, [this](const bool checked) {
        ui->spinBox_FilesToParse->setEnabled(checked);
    });

    connect(ui->checkBox_OccurrencesFoundLimit, &QCheckBox::toggled, this, [this](const bool checked) {
        ui->spinBox_OccurrencesFoundLimit->setEnabled(checked);
    });

    connect(ui->btn_ShowStatistics, &QPushButton::clicked, this, &MainWindow::showStatistics);

    connect(ui->btn_RescanResults, &QPushButton::clicked, this, [this]() {

        m_elapsedTimer.start();
        m_statsStartTime = QTime::currentTime().toString("hh:mm:ss");
        m_statsEndTime = "";
        m_statsElapsed = "";

        m_resultsModel->rescan(ui->checkBox_FileReadingTimeout->isChecked(),
                               ui->spinBox_FileReadingTimeout->value(),
                               ui->checkBox_OccurrencesFoundLimit->isChecked(),
                               ui->spinBox_OccurrencesFoundLimit->value(),
                               !m_filterWidget_FindText->dontMatch());

        m_statsElapsed = DateTime_Utils::formatElapsedTime(m_elapsedTimer.elapsed());
        m_statsEndTime = QTime::currentTime().toString("hh:mm:ss");

    });

    connect(ui->actionImport_Results, &QAction::triggered, this, [this]() {
        clearViews();
        HandleResults::importResults(*m_resultsModel, *m_appSettings, this);
    });

    connect(ui->actionExport_Results, &QAction::triggered, this, [this]() {
        HandleResults::exportResults(*m_resultsModel, *m_appSettings, this);
    });

    connect(ui->actionSettings, &QAction::triggered, this, &MainWindow::showSettings);

    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::aboutApplication);

    connect(ui->actionReport_a_problem, &QAction::triggered, this, [this]() {
        QDesktopServices::openUrl(QUrl("https://github.com/RachidTagzen/Text-Digger/issues"));
    });

}



// *********************************************************************************************************************
// ***************************************************** Functions *****************************************************
// *********************************************************************************************************************
void MainWindow::startSearch() {

    if (m_isSearching) {
        // If a search is already running, cancel it first
        cancelSearch();
        // Connect to the thread's finished signal to start a new search after the current one stops
        connect(m_findOccurrencesThread, &QThread::finished, this, &MainWindow::startSearch, Qt::SingleShotConnection);
        return;
    }


    // --------------------------
    // Initialize & clearing Lists
    // --------------------------
    if (!clearLists())
        return;


    // --------------------------
    // Prompt user if MinDepth exceeds MaxDepth
    // --------------------------
    if (ui->spinBox_MinDepth->value() > ui->spinBox_MaxDepth->value()) {
        QMessageBox::warning(this, "Warning", "The minimum depth is greater than the maximum depth.");
        return;
    }


    // --------------------------
    // Initialize other variables
    // --------------------------
    m_subdirectories = ui->checkBox_Subdirectories->isChecked();
    m_minDepth = ui->spinBox_MinDepth->value();
    m_maxDepth = ui->spinBox_MaxDepth->value();
    m_ignoreHiddenDirectories = ui->checkBox_IgnoreHiddenDirectories->isChecked();
    m_ignoreHiddenFiles = ui->checkBox_IgnoreHiddenFiles->isChecked();
    m_ignoreSymbolicDirectoriesLinks = ui->checkBox_IgnoreSymbolicDirectoriesLinks->isChecked();
    m_ignoreSymbolicFilesLinks = ui->checkBox_IgnoreSymbolicFilesLinks->isChecked();
    m_findExactFilename = ui->checkBox_FindExactFilename->isChecked();
    m_ignoreUnparseableFiles = ui->checkBox_IgnoreUnparseableFiles->isChecked();
    m_avoidDuplicates = ui->checkBox_AvoidDuplicateFiles->isChecked();

    m_sizeSystem = ui->comboBox_SizeSystems->currentText();
    m_sizeCondition = ui->comboBox_SizeConditions->currentText();
    m_size_1 = ui->lineEdit_Size_1->text().replace(",", ".").toDouble();
    m_size_2 = ui->lineEdit_Size_2->text().replace(",", ".").toDouble();
    m_sizeUnits_1 = ui->comboBox_SizeUnits_1->currentText();
    m_sizeUnits_2 = ui->comboBox_SizeUnits_2->currentText();
    m_creationDateCondition = ui->comboBox_CreationDateConditions->currentText();
    m_creationDate_1 = ui->dateTimeEdit_CreationDate_1->dateTime();
    m_creationDate_2 = ui->dateTimeEdit_CreationDate_2->dateTime();
    m_lastModificationCondition = ui->comboBox_LastModificationDateConditions->currentText();
    m_lastModificationDate_1 = ui->dateTimeEdit_LastModificationDate_1->dateTime();
    m_lastModificationDate_2 = ui->dateTimeEdit_LastModificationDate_2->dateTime();
    m_lastAccessDateCondition = ui->comboBox_LastAccessDateConditions->currentText();
    m_lastAccessDate_1 = ui->dateTimeEdit_LastAccessDate_1->dateTime();
    m_lastAccessDate_2 = ui->dateTimeEdit_LastAccessDate_1->dateTime();
    m_ignoreHiddenFiles = ui->checkBox_IgnoreHiddenFiles->isChecked();

    const bool filterBySize = ui->checkBox_Size->isChecked();
    const bool filterByCreationDate = ui->checkBox_CreationDate->isChecked();
    const bool filterByLastModificationDate = ui->checkBox_LastModificationDate->isChecked();
    const bool filterByLastAccessDate = ui->checkBox_LastAccessDate->isChecked();
    const bool filterByMimeTypes = ui->groupBox_MimeTypes->isChecked();

    m_fileReadingTimeout = ui->checkBox_FileReadingTimeout->isChecked();
    m_limitFilesToParse = ui->checkBox_FilesToParse->isChecked();
    m_limitOccurrencesFound = ui->checkBox_OccurrencesFoundLimit->isChecked();
    m_timeoutFileReading = ui->spinBox_FileReadingTimeout->value();
    m_filesToParseLimit = ui->spinBox_FilesToParse->value();
    m_occurrencesFoundLimit = ui->spinBox_OccurrencesFoundLimit->value();

    m_elapsedTimer.start();
    m_statsStartTime = QTime::currentTime().toString("hh:mm:ss");
    m_statsEndTime = "";
    m_statsElapsed = "";


    // --------------------------
    // Initialize Search Text variable
    // --------------------------
    QString searchText = m_filterWidget_FindText->text();
    FilterWidget::PatternSyntax patternSyntax_SearchText = m_filterWidget_FindText->patternSyntax();

    if (patternSyntax_SearchText == FilterWidget::Wildcard)
        searchText = QRegularExpression::wildcardToRegularExpression(searchText);
    else if (patternSyntax_SearchText == FilterWidget::FixedString)
        searchText = QRegularExpression::escape(searchText);

    // Add word boundaries (\b) around the searchText if the "Match whole words" checkbox is checked
    if (ui->checkBox_MatchWholeWords->isChecked())
        searchText = "\\b" + QRegularExpression::escape(searchText) + "\\b";

    searchText = searchText.normalized(QString::NormalizationForm_D);

    QRegularExpression::PatternOptions patternOptions = QRegularExpression::NoPatternOption;

    if (m_filterWidget_FindText->caseSensitivity() == Qt::CaseInsensitive)
        patternOptions |= QRegularExpression::CaseInsensitiveOption;

    m_searchTextPattern = QRegularExpression(searchText, patternOptions);

    m_dontMatchText = m_filterWidget_FindText->dontMatch();


    // --------------------------
    // Initialize Filenames variable
    // --------------------------
    m_targetFilenames = m_filterWidget_Filenames->text();
    QStringList filenamePatterns = m_targetFilenames.split(';');
    QVector<QRegularExpression>().swap(m_filenamesPatterns);

    const Qt::CaseSensitivity filenamesCaseSensitivity = m_filterWidget_Filenames->caseSensitivity();

    FilterWidget::PatternSyntax patternSyntax_Filenames = m_filterWidget_Filenames->patternSyntax();

    for (QString &filenamePattern : filenamePatterns) {

        if (patternSyntax_Filenames == FilterWidget::Wildcard)
            filenamePattern = QRegularExpression::wildcardToRegularExpression(filenamePattern);
        else if (patternSyntax_Filenames == FilterWidget::FixedString)
            filenamePattern = QRegularExpression::escape(filenamePattern);

        QRegularExpression::PatternOptions patternOptions = QRegularExpression::NoPatternOption;


        if (filenamesCaseSensitivity == Qt::CaseInsensitive)
            patternOptions |= QRegularExpression::CaseInsensitiveOption;

        m_filenamesPatterns.append(QRegularExpression(filenamePattern, patternOptions));
    }

    m_dontMatchfilenames = m_filterWidget_Filenames->dontMatch();


    // --------------------------
    //
    // --------------------------
    m_findOccurrencesWorker = new FindOccurrences(m_checkedDirectoriesToInclude, m_checkedDirectoriesToExclude,
                                                  m_checkedMimeTypes, m_filesList, m_resultsModel, m_searchTextPattern,
                                                  m_targetFilenames, m_filenamesPatterns, patternSyntax_Filenames,
                                                  filenamesCaseSensitivity, !m_dontMatchText, m_dontMatchfilenames,
                                                  m_subdirectories, m_minDepth, m_maxDepth, m_ignoreHiddenDirectories,
                                                  m_ignoreHiddenFiles, m_ignoreSymbolicDirectoriesLinks,
                                                  m_ignoreSymbolicFilesLinks, m_findExactFilename,
                                                  m_ignoreUnparseableFiles, m_avoidDuplicates, m_filesHashes_Set,
                                                  m_filtersDirectories, m_filtersFiles, m_sizeCondition, m_sizeSystem,
                                                  m_size_1, m_size_2, m_sizeUnits_1, m_sizeUnits_2,
                                                  m_creationDateCondition, m_creationDate_1, m_creationDate_2,
                                                  m_lastModificationCondition, m_lastModificationDate_1,
                                                  m_lastModificationDate_2, m_lastAccessDateCondition, m_lastAccessDate_1,
                                                  m_lastAccessDate_2, filterBySize, filterByCreationDate,
                                                  filterByLastModificationDate, filterByLastAccessDate, filterByMimeTypes,
                                                  m_fileReadingTimeout,  m_limitFilesToParse, m_limitOccurrencesFound,
                                                  m_timeoutFileReading, m_filesToParseLimit, m_occurrencesFoundLimit,
                                                  nullptr);

    m_findOccurrencesThread = new QThread;
    m_findOccurrencesWorker->moveToThread(m_findOccurrencesThread);


    // --------------------------
    // Connects
    // --------------------------
    connect(m_findOccurrencesThread, &QThread::started, m_findOccurrencesWorker, &FindOccurrences::start);
    connect(m_findOccurrencesWorker, &FindOccurrences::finished, m_findOccurrencesThread, &QThread::quit);
    connect(m_findOccurrencesWorker, &FindOccurrences::canceled, m_findOccurrencesThread, &QThread::quit);
    connect(m_findOccurrencesWorker, &FindOccurrences::finished, m_findOccurrencesWorker, &FindOccurrences::deleteLater);
    connect(m_findOccurrencesWorker, &FindOccurrences::canceled, m_findOccurrencesWorker, &FindOccurrences::deleteLater);
    connect(m_findOccurrencesThread, &QThread::finished, m_findOccurrencesThread, &QThread::deleteLater);

    connect(m_findOccurrencesWorker, &FindOccurrences::finished, this, [this](const QMap<QString, qint64> &statisticsMap) {
        m_statisticsMap = statisticsMap;
        searchFinished();
    }, Qt::QueuedConnection);

    connect(m_findOccurrencesWorker, &FindOccurrences::canceled, this, [this](const QMap<QString, qint64> &statisticsMap) {
        m_statisticsMap = statisticsMap;
        searchCanceled();
    }, Qt::QueuedConnection);

    connect(m_findOccurrencesWorker, &FindOccurrences::updateStatusBarOperation, this, [this](const QString &operation) {
        m_statusBarWidget->setOperation(operation);
    }, Qt::QueuedConnection);

    connect(m_findOccurrencesWorker, &FindOccurrences::updateStatusBarMessage, this, [this](const QString &message) {
        m_statusBarWidget->setMessage(message);
    }, Qt::QueuedConnection);


    // --------------------------
    // --------------------------
    disableControls(true);
    m_isSearching = true;
    ui->btn_StartSearch->setIcon(AppIcons::getIcon(IconType::CANCEL));
    m_findOccurrencesThread->start();

}


void MainWindow::cancelSearch() {

    if (m_findOccurrencesWorker) {
        // Signal the worker to cancel the operation
        m_findOccurrencesWorker->cancel();

        // Wait for the worker to finish processing and for the thread to stop
        m_findOccurrencesThread->quit();
        m_findOccurrencesThread->wait();
    }

    searchFinished();
}


void MainWindow::searchFinished() {

    m_statsElapsed = DateTime_Utils::formatElapsedTime(m_elapsedTimer.elapsed());
    m_statsEndTime = QTime::currentTime().toString("hh:mm:ss");

    disableControls(false);
    m_isSearching = false;
    ui->btn_StartSearch->setIcon(AppIcons::getIcon(IconType::SEARCH));
    m_statusBarWidget->clearOperation();
    m_statusBarWidget->setMessage("Searching operation successfully finished.");
    qDebug() << "Searching operation successfully finished.";
}


void MainWindow::searchCanceled() {

    m_statsElapsed = DateTime_Utils::formatElapsedTime(m_elapsedTimer.elapsed());
    m_statsEndTime = QTime::currentTime().toString("hh:mm:ss");

    disableControls(false);
    m_isSearching = false;
    ui->btn_StartSearch->setIcon(AppIcons::getIcon(IconType::SEARCH));
    m_statusBarWidget->clearOperation();
    m_statusBarWidget->setMessage("Searching operation canceled.");
    qDebug() << "Searching operation canceled";
}


bool MainWindow::clearLists() {

    // Clear the Set and free up the memory
    QSet<QString>().swap(m_checkedDirectoriesToInclude);
    for (int row = 0; row < m_includedDirectoriesModel->rowCount(); ++row) {
        const bool isChecked = m_includedDirectoriesModel->data(m_includedDirectoriesModel->index(row, 1),
                                                                Qt::CheckStateRole).toInt() == Qt::Checked;

        const QString directory = m_includedDirectoriesModel->data(m_includedDirectoriesModel->index(row, 2)).toString();
        if(isChecked)
            m_checkedDirectoriesToInclude << directory;
    }

    if (m_checkedDirectoriesToInclude.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select at least one directory to include.");
        return false;
    }


    QSet<QString>().swap(m_checkedDirectoriesToExclude);
    if (ui->groupBox_ExcludeDirectories->isChecked()) {
        for (int row = 0; row < m_excludedDirectoriesModel->rowCount(); ++row) {
            const bool isChecked = m_excludedDirectoriesModel->data(m_excludedDirectoriesModel->index(row, 1),
                                                                    Qt::CheckStateRole).toInt() == Qt::Checked;

            const QString directory = m_excludedDirectoriesModel->data(m_excludedDirectoriesModel->index(row, 2)).toString();
            if(isChecked)
                m_checkedDirectoriesToExclude << directory;
        }
    }


    const QMimeDatabase mimeDatabase;
    QSet<QMimeType>().swap(m_checkedMimeTypes);
    if (ui->groupBox_MimeTypes->isChecked()) {
        for (int row = 0; row < m_mimetypesModel->rowCount(); ++row) {
            const bool isChecked = m_mimetypesModel->data(m_mimetypesModel->index(row, 1),
                                                          Qt::CheckStateRole).toInt() == Qt::Checked;

            const QString mimeTypeName = m_mimetypesModel->data(m_mimetypesModel->index(row, 2)).toString();
            const QMimeType mimeType = mimeDatabase.mimeTypeForName(mimeTypeName);

            if (isChecked && mimeType.isValid())
                m_checkedMimeTypes.insert(mimeType);
        }

        if (m_checkedMimeTypes.isEmpty()) {
            QMessageBox::warning(this, "Warning", "Please select at least one MIME type.");
            return false;
        }
    }


    m_resultsModel->clearModel();

    QStringList().swap(m_filesList);
    // or fileList = QStringList();

    QSet<QString>().swap(m_filesHashes_Set);

    ui->textEdit_View->clear();

    return true;
}



// *********************************************************************************************************************
// *************************************************** View Results ****************************************************
// *********************************************************************************************************************
void MainWindow::viewResults(const QModelIndex &modelIndex) {

    clearViews();

    if (!modelIndex.isValid())
        return;

    // --------------------------
    // Fetch data from model
    // --------------------------
    const QStandardItem *fileItem = m_resultsModel->item(modelIndex.row(), 4);
    const QStandardItem *occurrencesItem = m_resultsModel->item(modelIndex.row(), 10);
    // const QRegularExpression searchTextPattern(m_resultsModel->item(modelIndex.row(), 11)->text());
    const QStandardItem *searchTextItem = m_resultsModel->item(modelIndex.row(), 11);





    const QString searchText = searchTextItem->data(Qt::UserRole + 2).toString();

    QRegularExpression::PatternOptions patternOptions = QRegularExpression::NoPatternOption;

    if (searchTextItem->data(Qt::UserRole + 1).toString() == "1")
        patternOptions |= QRegularExpression::CaseInsensitiveOption;

    QRegularExpression searchTextPattern = QRegularExpression(searchText, patternOptions);





    const int occurrencesCount = occurrencesItem->data(Qt::UserRole + 1).toInt();



    if (occurrencesCount == 0) {
        ui->textEdit_ViewHeader->setDisabled(occurrencesCount == 0);
        ui->textEdit_View->setDisabled(occurrencesCount == 0);
        return;
    }

    if (occurrencesCount > 1000) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "Large Data Warning",
            QString("You are about to open %1 occurrence%2, which may significantly slow down or freeze your system."
                    "\n\nAre you sure you want to proceed?")
                .arg(occurrencesItem->text(), occurrencesItem->text().toInt() != 1 ? "s" : ""),
            QMessageBox::Yes | QMessageBox::No
            );

        if (reply == QMessageBox::No)
            return;
    }


    // Update the text edit with the file name and occurrences
    ui->textEdit_ViewHeader->clear();
    ui->textEdit_ViewHeader->append(QString("<b>File : </b>%1").arg(fileItem->text()));
    ui->textEdit_ViewHeader->append(QString("<b>Occurrences : </b>%1").arg(occurrencesItem->text()));


    // --------------------------
    // Collect line numbers and occurrences
    // --------------------------
    QString filePath = fileItem->text();
    const QSet<int> lineNumbers_Set = occurrencesItem->data(Qt::UserRole + 2).value<QSet<int>>();

    // Convert the QSet to a QList and sort it
    QList<int> lineNumbers(lineNumbers_Set.begin(), lineNumbers_Set.end());
    std::sort(lineNumbers.begin(), lineNumbers.end());


    // --------------------------
    // Read the file and collect specified lines
    // --------------------------
    QFile file(filePath);

    // Check if the file exists before trying to open it
    if (!file.exists()) {
        QMessageBox::warning(nullptr, "File Error", "The file does not exist : " + filePath);
        qDebug() << "File does not exist : " << filePath;
        return;
    }

    // Try to open the file
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "File Error", "Failed to open file : " + filePath);
        qDebug() << "Failed to open file : " << filePath;
        return;
    }



    QMap<int, QString> lines;  // Map to store line number and the corresponding line content
    QTextStream in(&file);
    int lineNumber = 1;

    while (!in.atEnd()) {
        QString line = in.readLine();

        // Check if the current line number is in the list of lines we want
        if (lineNumbers.contains(lineNumber))
            lines.insert(lineNumber, line);

        lineNumber++;
    }

    // Close the file after reading
    file.close();


    // --------------------------
    // Display the lines and highlight the occurrences in QTextEdit
    // --------------------------
    QVector<QString> parts;

    // Iterate through each line in the QMap
    for (auto it = lines.constBegin(); it != lines.constEnd(); ++it) {
        const int lineNumber = it.key();
        QStringView lineText = QStringView(it.value());  // Use QStringView for a lightweight reference

        parts.append(QString("<span style='font-weight: bold;'>Line %1:</span><br/>").arg(lineNumber));

        QRegularExpressionMatchIterator matchIterator = searchTextPattern.globalMatchView(lineText);


        int lastIndex = 0;

        while (matchIterator.hasNext()) {
            const QRegularExpressionMatch match = matchIterator.next();

            // Debugging check for captured indices
            if (match.capturedStart() < 0 || match.capturedStart() > lineText.size() ||
                match.capturedEnd() < 0 || match.capturedEnd() > lineText.size()) {
                qWarning() << "Out of bounds : " << match.capturedStart() << match.capturedEnd() << lineText.size();
                break;
            }

            // Append text before the match
            if (match.capturedStart() > lastIndex)
                parts.append(QString(lineText.mid(lastIndex, match.capturedStart() - lastIndex)));

            // Append the matched text with HTML highlighting
            parts.append(QString("<span style='background-color: yellow;'>%1</span>").arg(match.captured()));

            lastIndex = match.capturedEnd();
        }

        // Append any remaining text after the last match
        if (lastIndex < lineText.size())
            parts.append(QString(lineText.mid(lastIndex)));


        parts.append("<br/><br/>");
    }


    ui->textEdit_View->setHtml(parts.join(""));


    // --------------------------
    // Scroll to top
    // --------------------------
    ui->textEdit_View->moveCursor(QTextCursor::Start);

}


void MainWindow::clearViews() {
    ui->textEdit_ViewHeader->setText("<b>File : </b>"
                                     "<br>"
                                     "<b>Occurrences : </b>");
    ui->textEdit_View->clear();
}



// *********************************************************************************************************************
// *************************************************** Miscellaneous ***************************************************
// *********************************************************************************************************************
void MainWindow::openFileOnExternalApplication(const QModelIndex &index) {

    if (!index.isValid() || !m_resultsSortFilterProxyModel) {
        QMessageBox::warning(this, "Warning", "Invalid file selection.");
        return;
    }

    const QString filePath = ui->tableView_Results->model()->index(index.row(), 4).data(Qt::DisplayRole).toString();
    OpenFiles::openFile(filePath, 10000000, this);
}


void MainWindow::openFilesOnExternalApplication(const SelectionType selectionType) {
    const QStringList filesToOpen = getFilePaths(selectionType);

    if (filesToOpen.isEmpty())
        return;

    OpenFiles::openFiles(filesToOpen, 10000000, 10, this);
}


void MainWindow::copyFilesPaths(const SelectionType &selectionType) {
    const QStringList filesPathsToCopy = getFilePaths(selectionType);

    if (filesPathsToCopy.isEmpty())
        return;

    CopyFiles::copyFilesPaths(filesPathsToCopy, this);
}



void MainWindow::deleteFiles(const SelectionType &selectionType) {
    const QStringList filesToDelete = getFilePaths(selectionType);

    if (filesToDelete.isEmpty())
        return;       

    QStringList successfullyDeleted = DeleteFiles::deleteFiles(filesToDelete,
                                                               m_appSettings->enableLoggers(),
                                                               m_appSettings->getLoggersFilesToKeep(),
                                                               this);


    if (successfullyDeleted.isEmpty())
        return;

    QItemSelectionModel *selectionModel = ui->tableView_Results->selectionModel();

    for (int row = m_resultsModel->rowCount() - 1; row >= 0; --row) {
        QString filePath = m_resultsModel->item(row, 4)->text();
        QFileInfo fileInfo(filePath);

        if (!fileInfo.exists()) {
            QModelIndex index = m_resultsModel->index(row, 0);
            selectionModel->select(index, QItemSelectionModel::Deselect);
            m_resultsModel->removeRow(row);
        }
    }

    clearViews();
}


void MainWindow::replaceContent(const SelectionType &selectionType) {

    const QStringList filesToReplace = getFilePaths(selectionType);

    if (filesToReplace.isEmpty())
        return;

    QStringList successfullyModifiedFiles = ReplaceOccurrences::replaceContent(filesToReplace,
                                                                               m_searchTextPattern,
                                                                               m_appSettings->enableLoggers(),
                                                                               m_appSettings->getLoggersFilesToKeep(),
                                                                               this);

    if (successfullyModifiedFiles.isEmpty())
        return;

    // Use a QSet for faster lookups on successfully modified files
    const QSet<QString> modifiedFilesSet(successfullyModifiedFiles.begin(), successfullyModifiedFiles.end());

    for (int row = m_resultsModel->rowCount() - 1; row >= 0; --row) {
        QString filePath = m_resultsModel->item(row, 4)->text();

        if (modifiedFilesSet.contains(filePath) || !QFileInfo::exists(filePath))
            m_resultsModel->removeRow(row);
    }

    clearViews();
}


QStringList MainWindow::getFilePaths(const SelectionType selectionType) {

    // 1 - QMessageBox::warning(this, "Warning", "No checked items.");
    // 2 - QMessageBox::warning(this, "Warning", "No valid files found.");

    // Validate essential models
    if (!m_resultsSortFilterProxyModel || !ui->tableView_Results->selectionModel() || !m_resultsModel) {
        QMessageBox::warning(this, "Warning", "No file available.");
        return {};
    }

    const int rowCount = m_resultsSortFilterProxyModel->rowCount();
    if (rowCount == 0) {
        QMessageBox::warning(this, "Warning", "There are no files.");
        return {};
    }

    QStringList filePaths;
    auto model = ui->tableView_Results->model();

    // Lambda to safely add file paths
    auto addFilePath = [&](int row) {
        const QString filePath = model->index(row, 4).data(Qt::DisplayRole).toString();
        if (!filePath.isEmpty())
            filePaths.append(filePath);
    };

    QModelIndexList selectedIndexes;

    switch (selectionType) {
    case SelectionType::ALL:
        for (int row = 0; row < rowCount; ++row)
            addFilePath(row);

        break;

    case SelectionType::SELECTED:
        selectedIndexes = ui->tableView_Results->selectionModel()->selectedRows();
        if (selectedIndexes.isEmpty()) {
            QMessageBox::warning(this, "Warning", "No items are selected.");
            return {};
        }

        for (const QModelIndex &index : selectedIndexes)
            addFilePath(index.row());

        break;

    case SelectionType::CHECKED:
        for (int row = 0; row < rowCount; ++row)
            if (model->index(row, 2).data(Qt::CheckStateRole).toInt() == Qt::Checked)
                addFilePath(row);

        if (filePaths.isEmpty()) {
            QMessageBox::warning(this, "Warning", "No checked items.");
            return {};
        }

        break;

    default:
        return {};
    }

    if (filePaths.isEmpty())
        QMessageBox::warning(this, "Warning", "No valid files found.");

    return filePaths;
}


void MainWindow::disableControls(bool value) {

    m_filterWidget_FindText->setDisabled(value);

    ui->groupBox_DirectoriesOptions->setDisabled(value);
    ui->groupBox_ExcludeDirectories->setDisabled(value);
    ui->groupBox_IncludeDirectories->setDisabled(value);
    ui->groupBox_FilesOptions->setDisabled(value);
    ui->groupBox_FilesAttributes->setDisabled(value);
    ui->groupBox_MimeTypes->setDisabled(value);

    ui->tab_results->setDisabled(value);
}


// *********************************************************************************************************************
// *************************************************** Context Menus ***************************************************
// *********************************************************************************************************************
void MainWindow::handleContextMenus() {

    QVector<QTableView*> tableviews = {
        ui->tableView_Results,
        ui->tableView_IncludeDirectories,
        ui->tableView_ExcludeDirectories,
        ui->tableView_MimeTypes
    };

    for (QTableView *tableView : tableviews) {
        tableView->setContextMenuPolicy(Qt::CustomContextMenu);

        connect(tableView, &QTableView::customContextMenuRequested, this, [this, tableView](const QPoint &pos) {
            if (!tableView) return;

            QMenu *menu_Root = new QMenu(this);
            menu_Root->setStyleSheet(AppStyles::getStyle(StyleType::CONTEXTMENUS));

            // Define the actions common to all menus
            auto addCommonActions = [&](QMenu *menu) {
                QAction *action_SelectAll = menu->addAction(AppIcons::getIcon(IconType::SELECTED), "Select All");
                QAction *action_deselectAll = menu->addAction(AppIcons::getIcon(IconType::UNSELECTED), "Deselect All");
                QAction *action_InvertSelection = menu->addAction(AppIcons::getIcon(IconType::INVERT), "Invert Selection");
                menu->addSeparator();
                QAction *action_CheckAll = menu->addAction(AppIcons::getIcon(IconType::CHECKED), "Check All");
                QAction *action_UncheckAll = menu->addAction(AppIcons::getIcon(IconType::UNCHECKED), "Uncheck All");
                QAction *action_CheckSelected = menu->addAction(AppIcons::getIcon(IconType::CHECKED), "Check Selected");
                QAction *action_UncheckSelected = menu->addAction(AppIcons::getIcon(IconType::UNCHECKED), "Uncheck Selected");
                QAction *action_InvertChecked = menu->addAction(AppIcons::getIcon(IconType::INVERT), "Invert Checked");

                // Connect the actions to their respective slots
                auto connectActions = [this, tableView](QAction *action, CheckingType type) {
                    connect(action, &QAction::triggered, this, [this, tableView, type]() {
                        selectRows(tableView, type);
                    });
                };


                connectActions(action_SelectAll, CheckingType::SELECT_ALL);
                connectActions(action_deselectAll, CheckingType::DESELECT_ALL);
                connectActions(action_InvertSelection, CheckingType::INVERT_SELECTION);
                connectActions(action_CheckAll, CheckingType::CHECK_ALL);
                connectActions(action_UncheckAll, CheckingType::UNCHECK_ALL);
                connectActions(action_CheckSelected, CheckingType::CHECK_SELECTED);
                connectActions(action_UncheckSelected, CheckingType::UNCHECK_SELECTED);
                connectActions(action_InvertChecked, CheckingType::INVERT_CHECKED);

                action_SelectAll->setShortcut(QKeySequence::SelectAll);
                action_SelectAll->setShortcutContext(Qt::ApplicationShortcut);
                addAction(action_SelectAll);
            };


            // Add common actions for all table views except Results
            if (tableView != ui->tableView_Results) {
                addCommonActions(menu_Root);

            } else {
                // Create submenus for Results table view
                QMenu *submenuSelection = menu_Root->addMenu(AppIcons::getIcon(IconType::SELECTIONS_CHECKBOXES), "Selection");
                addCommonActions(submenuSelection);

                // Add other specific submenus for the Results table view
                auto addSpecificActions = [this, menu_Root](const QIcon &icon, const QString &text,
                                                            std::function<void(SelectionType)> func) {

                    QMenu *submenu = menu_Root->addMenu(icon, text);

                    // Capture `func` by value explicitly in each lambda
                    connect(submenu->addAction(AppIcons::getIcon(IconType::ALL), "All"), &QAction::triggered,
                            this, [func]() { func(SelectionType::ALL); });

                    connect(submenu->addAction(AppIcons::getIcon(IconType::SELECTED), "Selected"), &QAction::triggered,
                            this, [func]() { func(SelectionType::SELECTED); });

                    connect(submenu->addAction(AppIcons::getIcon(IconType::CHECKED), "Checked"), &QAction::triggered,
                            this, [func]() { func(SelectionType::CHECKED); });
                };


                // Add specific action submenus
                addSpecificActions(AppIcons::getIcon(IconType::OPENFILES), "Open Files in External Application",
                                   [this](SelectionType type) { openFilesOnExternalApplication(type); });

                addSpecificActions(AppIcons::getIcon(IconType::COPY), "Copy Files Paths",
                                   [this](SelectionType type) { copyFilesPaths(type); });

                addSpecificActions(AppIcons::getIcon(IconType::TRASH), "Delete Files",
                                   [this](SelectionType type) { deleteFiles(type); });

                addSpecificActions(AppIcons::getIcon(IconType::EDITFILE), "Replace Content (Experimental)",

                                   [this](SelectionType type) { replaceContent(type); });
            }

            menu_Root->exec(tableView->mapToGlobal(pos));
            menu_Root->deleteLater();
        });
    }


    // ---------------
    // Context menu for horizontalHeader
    // ---------------
    ui->tableView_Results->horizontalHeader()->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(ui->tableView_Results->horizontalHeader(), &QHeaderView::customContextMenuRequested,
            this, [this](const QPoint &pos) {

                QMenu contextMenu("Columns", this);

                // Add actions for each column in the table view
                for (int i = 0; i < 12; ++i) {
                    QAction *action = contextMenu.addAction(m_resultsModel->headerData(i, Qt::Horizontal).toString());
                    action->setCheckable(true);
                    action->setChecked(!ui->tableView_Results->isColumnHidden(i));

                    // Capture `i` by value to avoid issues with the loop variable going out of scope
                    connect(action, &QAction::toggled, this, [this, i](bool checked) {
                        ui->tableView_Results->setColumnHidden(i, !checked);
                    });
                }

                // Show the context menu at the specified position
                contextMenu.exec(ui->tableView_Results->horizontalHeader()->mapToGlobal(pos));
            });

}


void MainWindow::selectRows(QTableView *tableView, CheckingType checkingType) {

    QStandardItemModel *model = nullptr;
    int checkColumn = (tableView == ui->tableView_Results) ? 2 : 1;

    if (tableView == ui->tableView_IncludeDirectories)
        model = m_includedDirectoriesModel;
    else if (tableView == ui->tableView_ExcludeDirectories)
        model = m_excludedDirectoriesModel;
    else if (tableView == ui->tableView_MimeTypes)
        model = m_mimetypesModel;
    else if (tableView == ui->tableView_Results)
        model = m_resultsModel;


    QItemSelectionModel *selectionModel = tableView->selectionModel();
    if (!model || !selectionModel) return;

    clearViews();

    std::function<QModelIndex(const QModelIndex&)> getSourceIndex;
    if (tableView == ui->tableView_Results) {
        getSourceIndex = [&](const QModelIndex &proxyIndex) {
            return m_resultsSortFilterProxyModel->mapToSource(proxyIndex);
        };
    } else {
        getSourceIndex = [&](const QModelIndex &proxyIndex) {
            return proxyIndex;
        };
    }

    auto updateCheckState = [&](Qt::CheckState newState) {
        for (int row = 0; row < model->rowCount(); ++row)
            if (auto *item = model->item(row, checkColumn))
                item->setCheckState(newState);
    };

    auto handleCheckSelection = [&]() {
        std::vector<QStandardItem*> currentlySelectedItems;

        for (const QModelIndex &proxyIndex : selectionModel->selectedIndexes())
            currentlySelectedItems.push_back(model->itemFromIndex(getSourceIndex(proxyIndex)));

        for (int row = 0; row < model->rowCount(); ++row) {
            QStandardItem *item = model->item(row, checkColumn);
            if (item && item->checkState() == Qt::Checked) {
                QStandardItem *targetItem = model->item(row, 0);
                if (checkingType == CheckingType::SELECT_CHECKED)
                    currentlySelectedItems.push_back(targetItem);
                else if (checkingType == CheckingType::DESELECT_CHECKED)
                    currentlySelectedItems.erase(std::remove(currentlySelectedItems.begin(),
                                                             currentlySelectedItems.end(),
                                                             targetItem),
                                                 currentlySelectedItems.end());
            }
        }

        selectionModel->clearSelection();
        for (QStandardItem *selectedItem : currentlySelectedItems) {
            QModelIndex proxyIndex = tableView == ui->tableView_Results
                                         ? m_resultsSortFilterProxyModel->mapFromSource(selectedItem->index())
                                         : selectedItem->index();

            selectionModel->select(proxyIndex, QItemSelectionModel::Select);
        }
    };

    switch (checkingType) {
    case CheckingType::SELECT_ALL:
        tableView->selectAll();
        break;
    case CheckingType::DESELECT_ALL:
        tableView->clearSelection();
        break;
    case CheckingType::SELECT_CHECKED:
    case CheckingType::DESELECT_CHECKED:
        handleCheckSelection();
        break;
    case CheckingType::INVERT_SELECTION: {
        int rowsCount = tableView->model()->rowCount();
        selectionModel->blockSignals(true);
        for (int row = 0; row < rowsCount; ++row) {
            QModelIndex topLeft = tableView->model()->index(row, 0);
            QModelIndex bottomRight = tableView->model()->index(row, tableView->model()->columnCount() - 1);
            QItemSelection rowSelection(topLeft, bottomRight);
            selectionModel->select(rowSelection, selectionModel->isSelected(topLeft)
                                                     ? QItemSelectionModel::Deselect
                                                     : QItemSelectionModel::Select);
        }
        selectionModel->blockSignals(false);
        break;
    }
    case CheckingType::CHECK_ALL:
        updateCheckState(Qt::Checked);
        break;
    case CheckingType::UNCHECK_ALL:
        updateCheckState(Qt::Unchecked);
        break;
    case CheckingType::CHECK_SELECTED:
    case CheckingType::UNCHECK_SELECTED: {
        Qt::CheckState targetState = (checkingType == CheckingType::CHECK_SELECTED) ? Qt::Checked : Qt::Unchecked;
        for (const QModelIndex &proxyIndex : selectionModel->selectedIndexes()) {
            if (proxyIndex.column() == checkColumn)
                if (auto *item = model->itemFromIndex(getSourceIndex(proxyIndex)))
                    item->setCheckState(targetState);
        }
        break;
    }
    case CheckingType::INVERT_CHECKED:
        for (int row = 0; row < model->rowCount(); ++row)
            if (auto *item = model->item(row, checkColumn))
                item->setCheckState(item->checkState() == Qt::Checked ? Qt::Unchecked : Qt::Checked);
        break;
    default:
        break;
    }
}



// *********************************************************************************************************************
// *********************************************** Application Settings ************************************************
// *********************************************************************************************************************
const QString MainWindow::getSettingValue(const QVector<Store_Setting> &settingsList, const QString &key) {

    auto storeSetting = std::ranges::find_if(settingsList, [&key](const Store_Setting &storeSetting) {
        return storeSetting.getKey() == key;
    });

    if (storeSetting == settingsList.end())
        return "";

    return storeSetting->getValue().simplified().isEmpty() ? storeSetting->getDefaultValue() : storeSetting->getValue();
}


void MainWindow::loadSettings_FromDatabase() {

    QVector<Store_Setting> settingsList = m_database_Settings->load_Settings();

    m_appSettings->setFilenames(getSettingValue(settingsList, "m_filenames"));
    m_appSettings->setFilenamesCaseSensitive(getSettingValue(settingsList, "m_filenamesCaseSensitive").toInt());
    m_appSettings->setDontMatchfilenames(getSettingValue(settingsList, "m_dontMatchfilenames").toInt());
    m_appSettings->setFilenamesPatternSyntax(getSettingValue(settingsList, "m_filenamesPatternSyntax"));
    m_appSettings->setEnableFindExactFilename(getSettingValue(settingsList, "m_enableFindExactFilename").toInt());
    m_appSettings->setParseSubdirectories(getSettingValue(settingsList, "m_parseSubdirectories").toInt());
    m_appSettings->setMinDepth(getSettingValue(settingsList, "m_minDepth").toInt());
    m_appSettings->setMaxDepth(getSettingValue(settingsList, "m_maxDepth").toInt());
    m_appSettings->setIgnoreHiddenDirectories(getSettingValue(settingsList, "m_ignoreHiddenDirectories").toInt());
    m_appSettings->setIgnoreSymbolicDirectoriesLinks(getSettingValue(settingsList, "m_ignoreSymbolicDirectoriesLinks").toInt());
    m_appSettings->setLastOpenedIncludeDir(getSettingValue(settingsList, "m_lastOpenedIncludeDir"));
    m_appSettings->setDirectoriesToInclude(getSettingValue(settingsList, "m_directoriesToInclude"));
    m_appSettings->setExcludeDirectories(getSettingValue(settingsList, "m_excludeDirectories").toInt());
    m_appSettings->setLastOpenedExcludeDir(getSettingValue(settingsList, "m_lastOpenedExcludeDir"));
    m_appSettings->setDirectoriesToExclude(getSettingValue(settingsList, "m_directoriesToExclude"));
    m_appSettings->setTextToFind(getSettingValue(settingsList, "m_textToFind"));
    m_appSettings->setTextToFindCaseSensitive(getSettingValue(settingsList, "m_textToFindCaseSensitive").toInt());
    m_appSettings->setDontMatchText(getSettingValue(settingsList, "m_dontMatchText").toInt());
    m_appSettings->setTextToFindPatternSyntax(getSettingValue(settingsList, "m_textToFindPatternSyntax"));
    m_appSettings->setMatchWholeWords(getSettingValue(settingsList, "m_matchWholeWords").toInt());
    m_appSettings->setFilterBySize(getSettingValue(settingsList, "m_filterBySize").toInt());
    m_appSettings->setSizeSystems(getSettingValue(settingsList, "m_sizeSystems"));
    m_appSettings->setSizeConditions(getSettingValue(settingsList, "m_sizeConditions"));
    m_appSettings->setFileSizeUnits_1(getSettingValue(settingsList, "m_fileSizeUnits_1"));
    m_appSettings->setFileSizeUnits_2(getSettingValue(settingsList, "m_fileSizeUnits_2"));
    m_appSettings->setFileSize_1(getSettingValue(settingsList, "m_fileSize_1"));
    m_appSettings->setFileSize_2(getSettingValue(settingsList, "m_fileSize_2"));
    m_appSettings->setFilterByCreationDate(getSettingValue(settingsList, "m_filterByCreationDate").toInt());
    m_appSettings->setCreationDateConditions(getSettingValue(settingsList, "m_creationDateConditions"));
    m_appSettings->setCreationDate_1(getSettingValue(settingsList, "m_creationDate_1"));
    m_appSettings->setCreationDate_2(getSettingValue(settingsList, "m_creationDate_2"));
    m_appSettings->setFilterByLastModificationDate(getSettingValue(settingsList, "m_filterByLastModificationDate").toInt());
    m_appSettings->setLastModificationDateConditions(getSettingValue(settingsList, "m_lastModificationDateConditions"));
    m_appSettings->setLastModificationDate_1(getSettingValue(settingsList, "m_lastModificationDate_1"));
    m_appSettings->setLastModificationDate_2(getSettingValue(settingsList, "m_lastModificationDate_2"));
    m_appSettings->setFilterByLastAccessDate(getSettingValue(settingsList, "m_filterByLastAccessDate").toInt());
    m_appSettings->setLastAccessDateConditions(getSettingValue(settingsList, "m_lastAccessDateConditions"));
    m_appSettings->setLastAccessDate_1(getSettingValue(settingsList, "m_lastAccessDate_1"));
    m_appSettings->setLastAccessDate_2(getSettingValue(settingsList, "m_lastAccessDate_2"));
    m_appSettings->setIgnoreUnparseableFiles(getSettingValue(settingsList, "m_ignoreUnparseableFiles").toInt());
    m_appSettings->setIgnoreHiddenFiles(getSettingValue(settingsList, "m_ignoreHiddenFiles").toInt());
    m_appSettings->setIgnoreSymbolicFilesLinks(getSettingValue(settingsList, "m_ignoreSymbolicFilesLinks").toInt());
    m_appSettings->setAvoidDuplicateFiles(getSettingValue(settingsList, "m_avoidDuplicateFiles").toInt());
    m_appSettings->setEnableFileReadingTimeout(getSettingValue(settingsList, "m_enableFileReadingTimeout").toInt());
    m_appSettings->setFileReadingTimeout(getSettingValue(settingsList, "m_fileReadingTimeout").toInt());
    m_appSettings->setEnableFilesToParseLimit(getSettingValue(settingsList, "m_enableFilesToParseLimit").toInt());
    m_appSettings->setFilesToParseLimit(getSettingValue(settingsList, "m_filesToParseLimit").toInt());
    m_appSettings->setEnableOccurrencesFoundLimit(getSettingValue(settingsList, "m_enableOccurrencesFoundLimit").toInt());
    m_appSettings->setOccurrencesFoundLimit(getSettingValue(settingsList, "m_occurrencesFoundLimit").toInt());
    m_appSettings->setFilterByMimeTypes(getSettingValue(settingsList, "m_filterByMimeTypes").toInt());
    m_appSettings->setMimeTypesList(getSettingValue(settingsList, "m_mimeTypesList"));
    m_appSettings->setRememberSize(getSettingValue(settingsList, "m_rememberSize").toInt());
    m_appSettings->setWindowSize(getSettingValue(settingsList, "m_windowSize"));
    m_appSettings->setAlwaysOnTop(getSettingValue(settingsList, "m_alwaysOnTop").toInt());
    m_appSettings->setEnableLoggers(getSettingValue(settingsList, "m_enableLoggers").toInt());
    m_appSettings->setLoggersFilesToKeep(getSettingValue(settingsList, "m_LoggersFilesToKeep").toInt());
    m_appSettings->setEnableLoggers(getSettingValue(settingsList, "m_enableLoggers").toInt());
    m_appSettings->setLastResultsDirectory(getSettingValue(settingsList, "m_lastResultsDirectory"));

}


void MainWindow::saveSettings_ToDatabase() {

    QMap<QString, bool> includedDirectoriesMap;
    QMap<QString, bool> excludedDirectoriesMap;
    QMap<QString, bool> mimeTypesMap;

    for (int row = 0; row < m_includedDirectoriesModel->rowCount(); ++row) {
        const bool isChecked = m_includedDirectoriesModel->data(m_includedDirectoriesModel->index(row, 1),
                                                                Qt::CheckStateRole).toInt() == Qt::Checked;

        const QString directory = m_includedDirectoriesModel->data(m_includedDirectoriesModel->index(row, 2)).toString();
        includedDirectoriesMap.insert(directory, isChecked);
    }

    for (int row = 0; row < m_excludedDirectoriesModel->rowCount(); ++row) {
        const bool isChecked = m_excludedDirectoriesModel->data(m_excludedDirectoriesModel->index(row, 1),
                                                                Qt::CheckStateRole).toInt() == Qt::Checked;

        const QString directory = m_excludedDirectoriesModel->data(m_excludedDirectoriesModel->index(row, 2)).toString();
        excludedDirectoriesMap.insert(directory, isChecked);
    }

    for (int row = 0; row < m_mimetypesModel->rowCount(); ++row) {
        const bool isChecked = m_mimetypesModel->data(m_mimetypesModel->index(row, 1),
                                                      Qt::CheckStateRole).toInt() == Qt::Checked;

        const QString mimeType = m_mimetypesModel->data(m_mimetypesModel->index(row, 2)).toString();
        mimeTypesMap.insert(mimeType, isChecked);
    }


    m_appSettings->setFilenames(m_filterWidget_Filenames->text());
    m_appSettings->setFilenamesCaseSensitive(m_filterWidget_Filenames->caseSensitivity() == Qt::CaseSensitive);
    m_appSettings->setDontMatchfilenames(m_filterWidget_Filenames->dontMatch());
    m_appSettings->setFilenamesPatternSyntax(m_filterWidget_Filenames->patternSyntaxToString());
    m_appSettings->setEnableFindExactFilename(ui->checkBox_FindExactFilename->isChecked());
    m_appSettings->setParseSubdirectories(ui->checkBox_Subdirectories->isChecked());
    m_appSettings->setMinDepth(ui->spinBox_MinDepth->value());
    m_appSettings->setMaxDepth(ui->spinBox_MaxDepth->value());
    m_appSettings->setIgnoreHiddenDirectories(ui->checkBox_IgnoreHiddenDirectories->isChecked());
    m_appSettings->setIgnoreSymbolicDirectoriesLinks(ui->checkBox_IgnoreSymbolicDirectoriesLinks->isChecked());
    m_appSettings->setLastOpenedIncludeDir(m_lastOpenedIncludeDir);
    m_appSettings->setDirectoriesToInclude(includedDirectoriesMap);
    m_appSettings->setExcludeDirectories(ui->groupBox_ExcludeDirectories->isChecked());
    m_appSettings->setLastOpenedExcludeDir(m_lastOpenedExcludeDir);
    m_appSettings->setDirectoriesToExclude(excludedDirectoriesMap);
    m_appSettings->setTextToFind(m_filterWidget_FindText->text());
    m_appSettings->setTextToFindCaseSensitive(m_filterWidget_FindText->caseSensitivity() == Qt::CaseSensitive);
    m_appSettings->setDontMatchText(m_filterWidget_FindText->dontMatch());
    m_appSettings->setTextToFindPatternSyntax(m_filterWidget_FindText->patternSyntaxToString());
    m_appSettings->setMatchWholeWords(ui->checkBox_MatchWholeWords->isChecked());
    m_appSettings->setFilterBySize(ui->checkBox_Size->isChecked());
    m_appSettings->setSizeSystems(ui->comboBox_SizeSystems->currentText());
    m_appSettings->setSizeConditions(ui->comboBox_SizeConditions->currentText());
    m_appSettings->setFileSizeUnits_1(ui->comboBox_SizeUnits_1->currentText());
    m_appSettings->setFileSizeUnits_2(ui->comboBox_SizeUnits_2->currentText());
    m_appSettings->setFileSize_1(ui->lineEdit_Size_1->text());
    m_appSettings->setFileSize_2(ui->lineEdit_Size_2->text());
    m_appSettings->setFilterByCreationDate(ui->checkBox_CreationDate->isChecked());
    m_appSettings->setCreationDateConditions(ui->comboBox_CreationDateConditions->currentText());
    m_appSettings->setCreationDate_1(ui->dateTimeEdit_CreationDate_1->dateTime());
    m_appSettings->setCreationDate_2(ui->dateTimeEdit_CreationDate_2->dateTime());
    m_appSettings->setFilterByLastModificationDate(ui->checkBox_LastModificationDate->isChecked());
    m_appSettings->setLastModificationDateConditions(ui->comboBox_LastModificationDateConditions->currentText());
    m_appSettings->setLastModificationDate_1(ui->dateTimeEdit_LastModificationDate_1->dateTime());
    m_appSettings->setLastModificationDate_2(ui->dateTimeEdit_LastModificationDate_2->dateTime());
    m_appSettings->setFilterByLastAccessDate(ui->checkBox_LastAccessDate->isChecked());
    m_appSettings->setLastAccessDateConditions(ui->comboBox_LastAccessDateConditions->currentText());
    m_appSettings->setLastAccessDate_1(ui->dateTimeEdit_LastAccessDate_1->dateTime());
    m_appSettings->setLastAccessDate_2(ui->dateTimeEdit_LastAccessDate_2->dateTime());
    m_appSettings->setIgnoreUnparseableFiles(ui->checkBox_IgnoreUnparseableFiles->isChecked());
    m_appSettings->setIgnoreHiddenFiles(ui->checkBox_IgnoreHiddenFiles->isChecked());
    m_appSettings->setIgnoreSymbolicFilesLinks(ui->checkBox_IgnoreSymbolicFilesLinks->isChecked());
    m_appSettings->setAvoidDuplicateFiles(ui->checkBox_AvoidDuplicateFiles->isChecked());
    m_appSettings->setEnableFileReadingTimeout(ui->checkBox_FileReadingTimeout->isChecked());
    m_appSettings->setFileReadingTimeout(ui->spinBox_FileReadingTimeout->value());
    m_appSettings->setEnableFilesToParseLimit(ui->checkBox_FilesToParse->isChecked());
    m_appSettings->setFilesToParseLimit(ui->spinBox_FilesToParse->value());
    m_appSettings->setEnableOccurrencesFoundLimit(ui->checkBox_OccurrencesFoundLimit->isChecked());
    m_appSettings->setOccurrencesFoundLimit(ui->spinBox_OccurrencesFoundLimit->value());
    m_appSettings->setFilterByMimeTypes(ui->groupBox_MimeTypes->isChecked());
    m_appSettings->setMimeTypesList(mimeTypesMap);
    m_appSettings->setWindowSize(size());

    // -----------------------------
    // -----------------------------
    m_database_Settings->update_Settings(m_appSettings);
}


// *********************************************************************************************************************
// *********************************************** Initialiaze Controls ************************************************
// *********************************************************************************************************************
void MainWindow::initialiazeControls() {

    // ---------------------------
    // ---------------------------
    initialiazeControlsFromSettings();

    // ---------------------------
    // ---------------------------
    handleContextMenus();

    // -----------------------------
    // -----------------------------
    if (m_mimetypesModel->isEmpty())
        MimeTypes_Utils::reset(m_mimetypesModel, false);


    ui->btn_RescanResults->setToolTip(QString(RESCAN_MESSAGE)
                                          .arg(
                                              ui->checkBox_FileReadingTimeout->isChecked() ? "Enabled" : "Disabled",
                                              QString::number(ui->spinBox_FileReadingTimeout->value()),
                                              ui->checkBox_OccurrencesFoundLimit->isChecked() ? "Enabled" : "Disabled",
                                              QString::number(ui->spinBox_OccurrencesFoundLimit->value()),
                                              m_filterWidget_FindText->dontMatch() ? "Don't Match" : "Match"));

}


void MainWindow::initialiazeControlsFromSettings() {

    // ---------------------------
    // ---------------------------
    loadSettings_FromDatabase();


    // ---------------------------
    // ---------------------------
    if (m_appSettings->rememberSize())
        resize(m_appSettings->getWindowSize().width(), m_appSettings->getWindowSize().height());
    else
        setWindowState(Qt::WindowMaximized);


    m_lastOpenedIncludeDir = m_appSettings->getLastOpenedIncludeDir();

    m_lastOpenedExcludeDir = m_appSettings->getLastOpenedExcludeDir();

    m_filterWidget_Filenames->setText(m_appSettings->getFilenames());

    m_filterWidget_Filenames->setCaseSensitivity(m_appSettings->isFilenamesCaseSensitive()
                                                     ? Qt::CaseSensitive
                                                     : Qt::CaseInsensitive);

    m_filterWidget_Filenames->setDontMatch(m_appSettings->isDontMatchfilenames());

    m_filterWidget_Filenames->setPatternSyntax(m_filterWidget_Filenames->stringToPatternSyntax(m_appSettings->getFilenamesPatternSyntax()));

    ui->checkBox_FindExactFilename->setChecked(m_appSettings->enableFindExactFilename());

    ui->checkBox_Subdirectories->setChecked(m_appSettings->parseSubdirectories());

    ui->spinBox_MinDepth->setValue(m_appSettings->getMinDepth());

    ui->spinBox_MaxDepth->setValue(m_appSettings->getMaxDepth());

    ui->checkBox_IgnoreHiddenDirectories->setChecked(m_appSettings->ignoreHiddenDirectories());

    ui->checkBox_IgnoreSymbolicDirectoriesLinks->setChecked(m_appSettings->ignoreSymbolicDirectoriesLinks());

    ui->groupBox_ExcludeDirectories->setChecked(m_appSettings->excludeDirectories());

    m_filterWidget_FindText->setText(m_appSettings->getTextToFind());

    m_filterWidget_FindText->setCaseSensitivity(m_appSettings->isTextToFindCaseSensitive()
                                                    ? Qt::CaseSensitive
                                                    : Qt::CaseInsensitive);

    m_filterWidget_FindText->setDontMatch(m_appSettings->isDontMatchText());

    m_filterWidget_FindText->setPatternSyntax(m_filterWidget_FindText->stringToPatternSyntax(m_appSettings->getTextToFindPatternSyntax()));

    ui->checkBox_MatchWholeWords->setChecked(m_appSettings->matchWholeWords());

    ui->checkBox_Size->setChecked(m_appSettings->filterBySize());

    ui->comboBox_SizeSystems->setCurrentText(m_appSettings->getSizeSystems());

    ui->comboBox_SizeConditions->setCurrentText(m_appSettings->getSizeConditions());

    ui->comboBox_SizeUnits_1->setCurrentText(m_appSettings->getFileSizeUnits_1());

    ui->comboBox_SizeUnits_2->setCurrentText(m_appSettings->getFileSizeUnits_2());

    ui->lineEdit_Size_1->setText(QString::number(m_appSettings->getFileSize_1()));

    ui->lineEdit_Size_2->setText(QString::number(m_appSettings->getFileSize_1()));

    ui->checkBox_CreationDate->setChecked(m_appSettings->filterByCreationDate());

    ui->comboBox_CreationDateConditions->setCurrentText(m_appSettings->getCreationDateConditions());

    ui->dateTimeEdit_CreationDate_1->setDateTime(QDateTime::fromString(m_appSettings->getCreationDate_1(),
                                                                       "yyyy-MM-dd hh:mm:ss"));

    ui->dateTimeEdit_CreationDate_2->setDateTime(QDateTime::fromString(m_appSettings->getCreationDate_2(),
                                                                       "yyyy-MM-dd hh:mm:ss"));

    ui->checkBox_LastModificationDate->setChecked(m_appSettings->filterByLastModificationDate());

    ui->comboBox_LastModificationDateConditions->setCurrentText(m_appSettings->getLastModificationDateConditions());

    ui->dateTimeEdit_LastModificationDate_1->setDateTime(QDateTime::fromString(m_appSettings->getLastModificationDate_1(),
                                                                               "yyyy-MM-dd hh:mm:ss"));

    ui->dateTimeEdit_LastModificationDate_2->setDateTime(QDateTime::fromString(m_appSettings->getLastModificationDate_2(),
                                                                               "yyyy-MM-dd hh:mm:ss"));

    ui->checkBox_LastAccessDate->setChecked(m_appSettings->filterByLastAccessDate());

    ui->comboBox_LastAccessDateConditions->setCurrentText(m_appSettings->getLastAccessDateConditions());

    ui->dateTimeEdit_LastAccessDate_1->setDateTime(QDateTime::fromString(m_appSettings->getLastAccessDate_1(),
                                                                         "yyyy-MM-dd hh:mm:ss"));

    ui->dateTimeEdit_LastAccessDate_2->setDateTime(QDateTime::fromString(m_appSettings->getLastAccessDate_2(),
                                                                         "yyyy-MM-dd hh:mm:ss"));

    ui->checkBox_IgnoreUnparseableFiles->setChecked(m_appSettings->ignoreUnparseableFiles());

    ui->checkBox_IgnoreHiddenFiles->setChecked(m_appSettings->ignoreHiddenFiles());

    ui->checkBox_IgnoreSymbolicFilesLinks->setChecked(m_appSettings->ignoreSymbolicFilesLinks());

    ui->checkBox_AvoidDuplicateFiles->setChecked(m_appSettings->avoidDuplicateFiles());

    ui->checkBox_FileReadingTimeout->setChecked(m_appSettings->enableFileReadingTimeout());

    ui->spinBox_FileReadingTimeout->setValue(m_appSettings->getFileReadingTimeout());

    ui->checkBox_FilesToParse->setChecked(m_appSettings->enableFilesToParseLimit());

    ui->spinBox_FilesToParse->setValue(m_appSettings->getFilesToParseLimit());

    ui->checkBox_OccurrencesFoundLimit->setChecked(m_appSettings->enableOccurrencesFoundLimit());

    ui->spinBox_OccurrencesFoundLimit->setValue(m_appSettings->getOccurrencesFoundLimit());

    ui->groupBox_MimeTypes->setChecked(m_appSettings->filterByMimeTypes());


    const QStringList includedDirectoriesList = m_appSettings->getDirectoriesToInclude().split("\n", Qt::SkipEmptyParts);
    m_includedDirectoriesModel->clearModel();

    for (const QString &directory : includedDirectoriesList) {
        QString dirPath = directory.mid(3).trimmed();
        m_includedDirectoriesModel->appendNewItem(dirPath, directory.startsWith("[1]"), true);
    }

    m_includedDirectoriesModel->sort(2);



    const QStringList excludedDirectoriesList = m_appSettings->getDirectoriesToExclude().split("\n", Qt::SkipEmptyParts);
    m_excludedDirectoriesModel->clearModel();

    for (const QString &directory : excludedDirectoriesList) {
        QString dirPath = directory.mid(3).trimmed();
        m_excludedDirectoriesModel->appendNewItem(dirPath, directory.startsWith("[1]"), true);
    }

    m_excludedDirectoriesModel->sort(2);



    MimeTypes_Utils::restoreFromDatabase(m_appSettings->getMimeTypesList(), m_mimetypesModel, this);


    ui->spinBox_FileReadingTimeout->setEnabled(m_appSettings->enableFileReadingTimeout());

    ui->spinBox_FilesToParse->setEnabled(m_appSettings->enableFilesToParseLimit());

    ui->spinBox_OccurrencesFoundLimit->setEnabled(m_appSettings->enableOccurrencesFoundLimit());

    ui->comboBox_SizeSystems->setEnabled(m_appSettings->filterBySize());

    ui->comboBox_SizeConditions->setEnabled(m_appSettings->filterBySize());

    ui->comboBox_SizeUnits_1->setEnabled(m_appSettings->filterBySize());

    ui->comboBox_SizeUnits_2->setEnabled(m_appSettings->filterBySize());

    ui->comboBox_SizeUnits_2->setVisible(BETWEEN_REGEX.match(ui->comboBox_SizeConditions->currentText()).hasMatch());

    ui->lineEdit_Size_1->setEnabled(m_appSettings->filterBySize());

    ui->lineEdit_Size_2->setEnabled(m_appSettings->filterBySize());

    ui->lineEdit_Size_2->setVisible(BETWEEN_REGEX.match(ui->comboBox_SizeConditions->currentText()).hasMatch());

    ui->comboBox_CreationDateConditions->setEnabled(m_appSettings->filterByCreationDate());

    ui->dateTimeEdit_CreationDate_1->setEnabled(m_appSettings->filterByCreationDate());

    ui->dateTimeEdit_CreationDate_2->setEnabled(m_appSettings->filterByCreationDate());

    ui->dateTimeEdit_CreationDate_2->setVisible(BETWEEN_REGEX.match(ui->comboBox_CreationDateConditions->currentText()).hasMatch());

    ui->comboBox_LastModificationDateConditions->setEnabled(m_appSettings->filterByLastModificationDate());

    ui->dateTimeEdit_LastModificationDate_1->setEnabled(m_appSettings->filterByLastModificationDate());

    ui->dateTimeEdit_LastModificationDate_2->setEnabled(m_appSettings->filterByLastModificationDate());

    ui->dateTimeEdit_LastModificationDate_2->setVisible(BETWEEN_REGEX.match(ui->comboBox_LastModificationDateConditions->currentText()).hasMatch());

    ui->comboBox_LastAccessDateConditions->setEnabled(m_appSettings->filterByLastAccessDate());

    ui->dateTimeEdit_LastAccessDate_1->setEnabled(m_appSettings->filterByLastAccessDate());

    ui->dateTimeEdit_LastAccessDate_2->setEnabled(m_appSettings->filterByLastAccessDate());

    ui->dateTimeEdit_LastAccessDate_2->setVisible(BETWEEN_REGEX.match(ui->comboBox_LastAccessDateConditions->currentText()).hasMatch());

    ui->spinBox_MaxDepth->setEnabled(m_appSettings->parseSubdirectories());

    ui->spinBox_MinDepth->setEnabled(m_appSettings->parseSubdirectories());

}

