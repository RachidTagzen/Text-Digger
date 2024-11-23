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


#include "statisticswindow.h"
#include "ui_statisticswindow.h"
#include "constants/resources.h"

#include <QMainWindow>


StatisticsWindow::StatisticsWindow(StatisticsModel *statisticsModel, QWidget *parent)
    : QDialog(parent), ui(new Ui::StatisticsWindow) {

    ui->setupUi(this);


    setFixedSize(size()); // Set fixed size based on initial dimensions
    setWindowTitle("Statistics");


    // -----------------------------------------
    //  Model
    // -----------------------------------------
    ui->tableView_Statistics->setModel(statisticsModel);


    // -----------------------------------------
    // tableView_Statistics
    // -----------------------------------------
    ui->tableView_Statistics->setSelectionBehavior(QAbstractItemView::SelectRows);
    // // ui->tableView_Statistics->setShowGrid(false);
    ui->tableView_Statistics->setWordWrap(false);

    ui->tableView_Statistics->verticalHeader()->hide();
    ui->tableView_Statistics->verticalHeader()->setDefaultSectionSize(28);
    // // ui->tableView_Statistics->horizontalHeader()->hide();
    ui->tableView_Statistics->horizontalHeader()->setFixedHeight(30);
    ui->tableView_Statistics->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableView_Statistics->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableView_Statistics->setColumnWidth(0, 200);
    ui->tableView_Statistics->horizontalHeader()->setStretchLastSection(true);

    ui->tableView_Statistics->horizontalHeader()->setStyleSheet(AppStyles::getStyle(StyleType::GLOBAL));

}


StatisticsWindow::~StatisticsWindow() {
    delete ui;
}



