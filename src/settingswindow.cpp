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


#include "settingswindow.h"
#include "ui_settingswindow.h"

#include <QMainWindow>


SettingsWindow::SettingsWindow(App_Settings *&appSettings, QWidget *parent)
    : QDialog(parent), ui(new Ui::SettingsWindow), m_appSettings(appSettings) {

    ui->setupUi(this);


    setFixedSize(size()); // Set fixed size based on initial dimensions
    setWindowTitle("About");


    // -----------------------------------------
    //
    // -----------------------------------------
    connect(ui->checkBox_EnableLoggers, &QCheckBox::toggled, this, [this](const bool checked) {
        ui->spinBox_LoggerFilesToKeep->setEnabled(checked);
    });


    // -----------------------------------------
    //
    // -----------------------------------------
    ui->checkBox_RememberSize->setChecked(m_appSettings->rememberSize());
    ui->checkBox_AlwaysOnTop->setChecked(m_appSettings->alwaysOnTop());

    ui->checkBox_EnableLoggers->setChecked(m_appSettings->enableLoggers());
    ui->spinBox_LoggerFilesToKeep->setValue(m_appSettings->getLoggersFilesToKeep());
}


SettingsWindow::~SettingsWindow() {
    delete ui;
}


void SettingsWindow::closeEvent(QCloseEvent *event) {
    // Task to do before closing window

    m_appSettings->setRememberSize(ui->checkBox_RememberSize->isChecked());
    m_appSettings->setAlwaysOnTop(ui->checkBox_AlwaysOnTop->isChecked());
    m_appSettings->setEnableLoggers(ui->checkBox_EnableLoggers->isChecked());
    m_appSettings->setLoggersFilesToKeep(ui->spinBox_LoggerFilesToKeep->value());

    event->accept();
}

