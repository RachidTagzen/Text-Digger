#pragma once

#include "enumerators/enums.h"

#include <QFile>
#include <QIcon>



// *********************************************************************************************************************
// ***************************************************** Strings *******************************************************
// *********************************************************************************************************************
namespace AppStrings {

inline const QString &getString(StringType type) {
    static const std::map<StringType, QString> stringPaths = {
        {TOOLTIP_FINDTEXT, ":/strings/resources/strings/tooltip_filterwidget_findtext"},
        {TOOLTIP_FILENAMES, ":/strings/resources/strings/tooltip_filterwidget_filenames"},
        {TOOLTIP_SIZESYSTEM, ":/strings/resources/strings/tooltip_comboBox_sizesystems"},
        {TOOLTIP_SIZEUNITS, ":/strings/resources/strings/tooltip_comboBox_sizeunits"},
        {HOMEPAGE, ":/strings/resources/strings/homepage"},
        {FEEDBACK, ":/strings/resources/strings/feedback"},
        {MIMETYPES, ":/miscellaneous/resources/miscellaneous/parseable_mimetypes"}
    };

    static std::map<StringType, QString> strings;

    auto it = strings.find(type);
    if (it == strings.end()) {
        // Load the style if not already loaded
        auto pathIt = stringPaths.find(type);
        if (pathIt != stringPaths.end()) {
            QFile file(pathIt->second);
            if (file.open(QFile::ReadOnly)) {
                it = strings.emplace(type, QString::fromUtf8(file.readAll())).first;
                file.close();
            }
        }
    }

    if (it != strings.end())
        return it->second;

    static const QString emptyString; // Return an empty string if the style can't be found
    return emptyString;
}

}



// *********************************************************************************************************************
// ****************************************************** Styles *******************************************************
// *********************************************************************************************************************
namespace AppStyles {

inline const QString &getStyle(StyleType type) {
    static const std::map<StyleType, QString> stylePaths = {
        {GLOBAL, ":/styles/resources/styles/global.qss"},
        {CONTEXTMENUS, ":/styles/resources/styles/context_menus.qss"}
    };

    static std::map<StyleType, QString> styles;

    auto it = styles.find(type);
    if (it == styles.end()) {
        // Load the style if not already loaded
        auto pathIt = stylePaths.find(type);
        if (pathIt != stylePaths.end()) {
            QFile file(pathIt->second);
            if (file.open(QFile::ReadOnly)) {
                it = styles.emplace(type, QString::fromUtf8(file.readAll())).first;
                file.close();
            }
        }
    }

    if (it != styles.end())
        return it->second;

    static const QString emptyStyle; // Return an empty string if the style can't be found
    return emptyStyle;
}

}


// *********************************************************************************************************************
// ******************************************************* Icons *******************************************************
// *********************************************************************************************************************

namespace AppIcons {

inline const QIcon &getIcon(IconType type) {

    static const std::map<IconType, QString> iconPaths = {
        {SELECTED, ":/icons/resources/icons/svg/selected.svg"},
        {UNSELECTED, ":/icons/resources/icons/svg/unselected.svg"},
        {INVERT, ":/icons/resources/icons/svg/invert.svg"},
        {CHECKED, ":/icons/resources/icons/svg/checked.svg"},
        {UNCHECKED, ":/icons/resources/icons/svg/unchecked.svg"},
        {CANCEL, ":/icons/resources/icons/svg/cancel.svg"},
        {SEARCH, ":/icons/resources/icons/svg/search.svg"},
        {SELECTIONS_CHECKBOXES, ":/icons/resources/icons/svg/selections_checkboxes.svg"},
        {ALL, ":/icons/resources/icons/svg/all.svg"},
        {COPY, ":/icons/resources/icons/svg/copy.svg"},
        {TRASH, ":/icons/resources/icons/svg/trash.svg"},
        {OPENFILES, ":/icons/resources/icons/svg/open_files.svg"},
        {EDITFILE, ":/icons/resources/icons/svg/edit_file.svg"}
    };

    static std::map<IconType, QIcon> icons;

    auto it = icons.find(type);
    if (it == icons.end()) {
        // Create and store the icon if it doesn't exist
        auto pathIt = iconPaths.find(type);
        if (pathIt != iconPaths.end()) {
            it = icons.emplace(type, QIcon(pathIt->second)).first;
        } else {
            static const QIcon defaultIcon; // Optional: Default icon
            return defaultIcon;
        }
    }

    return it->second;
}

}
