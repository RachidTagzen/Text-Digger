/*
    Copyright (C) 2016 The Qt Company Ltd.
    SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

    Modifications (C) 2024 Rachid Tagzen
    SPDX-License-Identifier: BSD-3-Clause
*/


#include "filterwidget.h"

#include <QIcon>
#include <QPixmap>
#include <QMenu>
#include <QAction>
#include <QActionGroup>
#include <QToolButton>
#include <QWidgetAction>

FilterWidget::FilterWidget(QWidget *parent) : QLineEdit(parent) , m_patternGroup(new QActionGroup(this)) {

    setClearButtonEnabled(true);
    connect(this, &QLineEdit::textChanged, this, &FilterWidget::filterChanged);

    QMenu *menu = new QMenu(this);
    m_caseSensitivityAction = menu->addAction("Case Sensitive");
    m_caseSensitivityAction->setCheckable(true);
    connect(m_caseSensitivityAction, &QAction::toggled, this, &FilterWidget::filterChanged);

    m_dontMatchAction = menu->addAction("Don't Match");
    m_dontMatchAction->setCheckable(true);
    connect(m_dontMatchAction, &QAction::toggled, this, &FilterWidget::filterChanged);

    menu->addSeparator();
    m_patternGroup->setExclusive(true);

    m_regularExpressionAction = menu->addAction("Regular Expression");
    m_regularExpressionAction->setCheckable(true);
    m_regularExpressionAction->setChecked(true);
    m_regularExpressionAction->setData(QVariant(int(RegularExpression)));
    m_patternGroup->addAction(m_regularExpressionAction);

    m_wildcardAction = menu->addAction("Wildcard");
    m_wildcardAction->setCheckable(true);
    m_wildcardAction->setData(QVariant(int(Wildcard)));
    m_patternGroup->addAction(m_wildcardAction);

    m_fixedStringAction = menu->addAction("Fixed String");
    m_fixedStringAction->setData(QVariant(int(FixedString)));
    m_fixedStringAction->setCheckable(true);
    m_patternGroup->addAction(m_fixedStringAction);

    connect(m_patternGroup, &QActionGroup::triggered, this, &FilterWidget::filterChanged);

    connect(m_patternGroup, &QActionGroup::triggered, this, [this](QAction *action) {
        bool disableCheckbox = action->data().toInt() == RegularExpression || action->data().toInt() == Wildcard;
        emit disableExactMatchCheckbox(disableCheckbox);
    });

    const QIcon icon = QIcon(QPixmap(":/images/find.png"));
    QToolButton *optionsButton = new QToolButton;
#ifndef QT_NO_CURSOR
    optionsButton->setCursor(Qt::ArrowCursor);
#endif
    optionsButton->setFocusPolicy(Qt::NoFocus);
    optionsButton->setStyleSheet("* { border: none; }");
    optionsButton->setIcon(icon);
    optionsButton->setMenu(menu);
    optionsButton->setPopupMode(QToolButton::InstantPopup);

    QWidgetAction *optionsAction = new QWidgetAction(this);
    optionsAction->setDefaultWidget(optionsButton);
    addAction(optionsAction, QLineEdit::LeadingPosition);
}



Qt::CaseSensitivity FilterWidget::caseSensitivity() const {
    return m_caseSensitivityAction->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
}

void FilterWidget::setCaseSensitivity(Qt::CaseSensitivity cs) {
    m_caseSensitivityAction->setChecked(cs == Qt::CaseSensitive);
}

bool FilterWidget::dontMatch() const {
    return m_dontMatchAction->isChecked();
}

void FilterWidget::setDontMatch(bool exclude) {
    m_dontMatchAction->setChecked(exclude);
}

static inline FilterWidget::PatternSyntax patternSyntaxFromAction(const QAction *action) {
    return static_cast<FilterWidget::PatternSyntax>(action->data().toInt());
}

FilterWidget::PatternSyntax FilterWidget::patternSyntax() const {
    return patternSyntaxFromAction(m_patternGroup->checkedAction());
}

void FilterWidget::setPatternSyntax(PatternSyntax patternSyntax) {
    const QList<QAction*> actions = m_patternGroup->actions();
    for (QAction *a : actions) {
        if (patternSyntaxFromAction(a) == patternSyntax) {
            a->setChecked(true);
            break;
        }
    }
}

void FilterWidget::disablePatternsActions(bool disable) {
    m_regularExpressionAction->setEnabled(!disable);
    m_wildcardAction->setEnabled(!disable);
    m_fixedStringAction->setChecked(true);
}

// Convert current PatternSyntax to QString
QString FilterWidget::patternSyntaxToString() const {
    return patternSyntaxToString(patternSyntax());
}

// Convert PatternSyntax enum to QString
QString FilterWidget::patternSyntaxToString(PatternSyntax patternSyntax) const {
    switch (patternSyntax) {
    case RegularExpression:
        return QStringLiteral("RegularExpression");
    case Wildcard:
        return QStringLiteral("Wildcard");
    case FixedString:
        return QStringLiteral("FixedString");
    default:
        return QStringLiteral("Unknown");
    }
}

// Convert QString to PatternSyntax enum
FilterWidget::PatternSyntax FilterWidget::stringToPatternSyntax(const QString &str) const {

    if (str == QStringLiteral("RegularExpression"))
        return RegularExpression;

    if (str == QStringLiteral("Wildcard"))
        return Wildcard;

    if (str == QStringLiteral("FixedString"))
        return FixedString;

    // Default or error case, you could throw an error or return a default value
    return FixedString;  // Default to FixedString or handle this appropriately

}
