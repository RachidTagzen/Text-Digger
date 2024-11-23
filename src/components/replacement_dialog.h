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

#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>
#include <QMessageBox>


/**
 * The ReplacementDialog class is a dialog window that prompts the user for confirmation before replacing
 * content in multiple files. The user can specify replacement text, opt to create backup files, and choose
 * to ignore errors during the process.
 */
class ReplacementDialog : public QDialog {
    Q_OBJECT // Enables the use of signals and slots in this class


public:
    /**
     * Constructor for the ReplacementDialog class. Initializes the UI elements and sets up the dialog layout.
     *
     * @param filesToReplace The number of files that will be affected by the replacement.
     * @param parent The parent widget for this dialog (default is nullptr).
     */
    ReplacementDialog(int filesToReplace, QWidget *parent = nullptr)
        : QDialog(parent), m_layoutRoot(this), m_confirmationLabel(this), m_replacementLabel(this), m_lineEdit(this),
        m_backupCheckBox(this), m_ignoreErrorsCheckBox(this), m_buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this) {

        // ------------------------
        // Configure confirmation label
        // ------------------------
        m_confirmationLabel.setWordWrap(true);
        m_confirmationLabel.setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

        m_confirmationLabel.setText(QString(
                                        "<p>You are about to <strong>permanently</strong> replace all instances across %1 file%2.</p>"
                                        "<p style='color:red;'>This process will use the default character encoding"
                                        " (UTF-8) for reading and writing.</p>"
                                        "<p style='color:red;'>If any file has a different encoding, its content might "
                                        "not display correctly or could be corrupted.</p><hr />"
                                        )
                                        .arg(filesToReplace)
                                        .arg(filesToReplace > 1 ? "s" : ""));


        // ------------------------
        // Configure replacement label
        // ------------------------
        m_replacementLabel.setText("Enter replacement text : ");


        // ------------------------
        // Configure ignore errors checkbox
        // ------------------------
        m_ignoreErrorsCheckBox.setText("Ignore errors and continue");

        m_ignoreErrorsCheckBox.setToolTip(
            "Select this option to bypass errors and attempt to continue with the operation.\n\n"
            "<b>Note:</b> Enabling this option may result in incomplete or corrupted changes if errors occur. "
            "Use with caution, as files may be left in an inconsistent state.");


        // ------------------------
        // Configure backup checkbox
        // ------------------------
        m_backupCheckBox.setText(QString("Create backup file%1 before replacing").arg(filesToReplace > 1 ? "s" : ""));


        // ------------------------
        // Layout configuration
        // ------------------------
        m_layoutRoot.addWidget(&m_confirmationLabel);
        m_layoutRoot.addWidget(&m_replacementLabel);
        m_layoutRoot.addWidget(&m_lineEdit);
        m_layoutRoot.addWidget(&m_ignoreErrorsCheckBox);
        m_layoutRoot.addWidget(&m_backupCheckBox);
        m_layoutRoot.addWidget(&m_buttonBox);
        // m_layoutRoot.addStretch();


        // ------------------------
        // Signal connection
        // ------------------------
        connect(&m_buttonBox, &QDialogButtonBox::accepted, this, &ReplacementDialog::onAccept);
        connect(&m_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);


        // ------------------------
        //
        // ------------------------
        setWindowTitle("Confirm Replacement");
        setMinimumWidth(600);
        // setMaximumHeight(290);
        // adjustSize();

    }



    QString getReplacementText() const {
        return m_lineEdit.text();
    }


    bool isBackupSelected() const {
        return m_backupCheckBox.isChecked();
    }


    bool isIgnoreErrorsChecked() const {
        return m_ignoreErrorsCheckBox.isChecked();
    }


private slots:
    void onAccept() {
        if (m_lineEdit.text().isEmpty()) {
            QMessageBox::warning(this, "Warning", "Replacement text cannot be empty.");
            return;
        }

        accept();
    }


private:
    QVBoxLayout m_layoutRoot;
    QLabel m_confirmationLabel;
    QLabel m_replacementLabel;
    QLineEdit m_lineEdit;
    QCheckBox m_backupCheckBox;
    QCheckBox m_ignoreErrorsCheckBox;
    QDialogButtonBox m_buttonBox;


};
