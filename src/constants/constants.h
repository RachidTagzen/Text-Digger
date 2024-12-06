#pragma once

#include <QRegularExpression>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>


// *********************************************************************************************************************
// **************************************************** Information ****************************************************
// *********************************************************************************************************************
static QString APP_TITLE = "Text Digger";
static QString APP_VERSION = "1.0.0";
static QString APP_RELEASE_DATE = "06-12-2024 (16:58:00)";
static QString _QT_VERSION = "6.8.0";


// *********************************************************************************************************************
// ************************************************ Directories & Files ************************************************
// *********************************************************************************************************************
static const QDir HOME_DIRECTORY(QStandardPaths::writableLocation(QStandardPaths::HomeLocation));
static QDir SETTINGS_DIR(QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/" + APP_TITLE);
static QDir LOGGERS_DIR(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" + APP_TITLE + "/Loggers");
static QFileInfo SETTINGS_FILE(SETTINGS_DIR.filePath("settings.db"));


// *********************************************************************************************************************
// ******************************************************* Regex *******************************************************
// *********************************************************************************************************************
static const QRegularExpression BETWEEN_REGEX("^(Between|Not between|Exactly)$",
                                              QRegularExpression::CaseInsensitiveOption
                                                  | QRegularExpression::DotMatchesEverythingOption);

static const QRegularExpression SEPARATOR_WITH_WHITESPACE_REGEX("[;\\n\\s]+");

static const QRegularExpression SEPARATOR_WITHOUT_WHITESPACE_REGEX("[;\\n]+");


// *********************************************************************************************************************
// ****************************************************** Styles *******************************************************
// *********************************************************************************************************************
// static QString STYLE_SHEET;

// static const QString AAA(R"(
//                             QMenu::item:selected {
//                                 background-color: #3399FF;
//                             }
//                         )");



// *********************************************************************************************************************
// ****************************************************** Strings ******************************************************
// *********************************************************************************************************************
static const QString RESCAN_MESSAGE("Please ensure the 'Search' tab settings are configured correctly before rescanning.\n"
                                    "Current Settings :\n"
                                    " - File Reading Timeout : %1 (%2 ms)\n"
                                    " - Occurrences Found Limit : %3 (%4 occurrences)\n"
                                    " - Text Match : %5");




// *********************************************************************************************************************
// ******************************************************* Sizes *******************************************************
// *********************************************************************************************************************
static const QMap<QString, QStringList> SIZE_SYSTEM_AND_UNITS {
    {"SI",  {"Byte", "Bytes", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB"}},
    {"IEC", {"Byte", "Bytes", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB", "ZiB", "YiB"}}
};

static const QMap<QString, double> SI_CONVERSIONS {
    {"Byte", 1.0},
    {"Bytes", 1.0},
    {"KB", 1000.0},
    {"MB", 1000.0 * 1000.0},
    {"GB", 1000.0 * 1000.0 * 1000.0},
    {"TB", 1000.0 * 1000.0 * 1000.0 * 1000.0},
    {"PB", 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0},
    {"EB", 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0},
    {"ZB", 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0},
    {"YB", 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0 * 1000.0}
};

static const QMap<QString, double> IEC_CONVERSIONS {
    {"Byte", 1.0},
    {"Bytes", 1.0},
    {"KiB", 1024.0},
    {"MiB", 1024.0 * 1024.0},
    {"GiB", 1024.0 * 1024.0 * 1024.0},
    {"TiB", 1024.0 * 1024.0 * 1024.0 * 1024.0},
    {"PiB", 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0},
    {"EiB", 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0},
    {"ZiB", 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0},
    {"YiB", 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0 * 1024.0}
};



// *********************************************************************************************************************
// *************************************************** Miscellaneous ***************************************************
// *********************************************************************************************************************
static const QStringList CSV_HEADER({"√", "File", "Path", "Size", "Size System", "MIME Type", "Created", "Modified",
                                     "Accessed", "Founds", "lines", "Match Text", "Search Text Pattern", "Case Insensitive"});

