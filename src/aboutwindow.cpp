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


#include "aboutwindow.h"
#include "ui_aboutwindow.h"
#include "delegates/browsable_cell_delegate.h"
#include "constants/resources.h"

#include <QMainWindow>


AboutWindow::AboutWindow(QWidget *parent) : QDialog(parent), ui(new Ui::AboutWindow) {

    ui->setupUi(this);


    setFixedSize(size()); // Set fixed size based on initial dimensions
    setWindowTitle("About");


    ui->label_ApplicationVersion->setText(QString("v%1").arg(APP_VERSION));
    ui->label_Homepage->setText(AppStrings::getString(StringType::HOMEPAGE));
    ui->label_Feedback->setText(AppStrings::getString(StringType::FEEDBACK));


    // -----------------------------------------
    //  Models
    // -----------------------------------------
    StatisticsModel *detailsModel = new StatisticsModel(this, true);
    ui->tableView_Details->setModel(detailsModel);


    // -----------------------------------------
    // tableView_Statistics
    // -----------------------------------------
    ui->tableView_Details->setItemDelegate(new BrowsableCellDelegate());

    ui->tableView_Details->setSelectionBehavior(QAbstractItemView::SelectRows);
    // ui->tableView_Details->setShowGrid(false);
    ui->tableView_Details->setWordWrap(false);
    ui->tableView_Details->verticalHeader()->hide();
    ui->tableView_Details->verticalHeader()->setDefaultSectionSize(35);
    ui->tableView_Details->horizontalHeader()->setFixedHeight(30);
    ui->tableView_Details->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableView_Details->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableView_Details->setColumnWidth(0, 200);
    ui->tableView_Details->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_Details->horizontalHeader()->setStyleSheet(AppStyles::getStyle(StyleType::GLOBAL));


    // -----------------------------------------
    // Populate tableView_Details
    // -----------------------------------------
    detailsModel->appendNew("Application Title", APP_TITLE);
    detailsModel->appendNew("Application Version", APP_VERSION);
    detailsModel->appendNew("Application Release Date", APP_RELEASE_DATE);
    detailsModel->appendNew("Qt Version", _QT_VERSION);

    detailsModel->appendNew("Settings Directory", SETTINGS_DIR.absolutePath());
    detailsModel->appendNew("Loggers Directory", LOGGERS_DIR.absolutePath());

}


AboutWindow::~AboutWindow() {
    // Qt will automatically delete `detailsModel` because it’s parented to `AboutWindow`, because Qt will handle it here.
    delete ui;
}
