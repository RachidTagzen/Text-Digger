/*
    Copyright (C) 2016 The Qt Company Ltd.
    SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

    Modifications (C) 2024 Rachid Tagzen
    SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef FILTERWIDGET_H
#define FILTERWIDGET_H

#include <QLineEdit>

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
QT_END_NAMESPACE


class FilterWidget : public QLineEdit {
    Q_OBJECT
    Q_PROPERTY(Qt::CaseSensitivity caseSensitivity READ caseSensitivity WRITE setCaseSensitivity)
    Q_PROPERTY(PatternSyntax patternSyntax READ patternSyntax WRITE setPatternSyntax)


public:
    explicit FilterWidget(QWidget *parent = nullptr);

    Qt::CaseSensitivity caseSensitivity() const;
    void setCaseSensitivity(Qt::CaseSensitivity);

    bool dontMatch() const;
    void setDontMatch(bool exclude);

    enum PatternSyntax {
        RegularExpression,
        Wildcard,
        FixedString
    };
    Q_ENUM(PatternSyntax)

    PatternSyntax patternSyntax() const;
    void setPatternSyntax(PatternSyntax patternSyntax);

    // Conversion between PatternSyntax and QString
    QString patternSyntaxToString() const;
    QString patternSyntaxToString(PatternSyntax patternSyntax) const;
    PatternSyntax stringToPatternSyntax(const QString &str) const;


public slots:
    void disablePatternsActions(bool disable);


signals:
    void filterChanged();
    void disableExactMatchCheckbox(bool);


private:
    QAction *m_caseSensitivityAction;
    QAction *m_dontMatchAction;
    QActionGroup *m_patternGroup;
    QAction *m_regularExpressionAction;
    QAction *m_wildcardAction;
    QAction *m_fixedStringAction;

};

#endif // FILTERWIDGET_H
