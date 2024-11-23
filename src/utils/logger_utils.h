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

#include "constants/constants.h"
#include "utils/datetime_utils.h"

#include <QTextStream>
#include <QDateTime>


class Logger {

public:
    /**
     * Constructor for the Logger class.
     * Initializes the logger with a specified loggerType and maximum number of log files to retain.
     * Creates the initial log file and ensures that the log file limit is maintained.
     * @param loggerType - The loggerType of logging (DELETE or REPLACE).
     * @param maxFiles - The maximum number of log files to keep (default is 50).
     */
    explicit Logger(const LoggerType loggerType, const int maxFiles = 50) : maxFiles(maxFiles), loggerType(loggerType) {
        createLogFile();  // Create the initial log file for logging
        maintainLogFileLimit();  // Ensure only the latest `maxFiles` log files are kept
    }


    /**
     * Destructor for the Logger class.
     * Closes the log file upon destruction to ensure all data is flushed and resources are released.
     */
    ~Logger() {
        logFile.close();
    }


    /**
     * Logs an entry to the log file with the specified details.
     * Each entry consists of the current date, time, status, operation, and the associated file.
     * @param status - The status of the operation (e.g., success, error).
     * @param operation - A description of the operation being logged.
     * @param file - The name of the file associated with the operation.
     */
    void log(const QString &status, const QString &operation, const QString &file) {
        if (logFile.isOpen()) {
            QTextStream out(&logFile);
            QDateTime now = QDateTime::currentDateTime();
            out << "\"" << now.toString("yyyy-MM-dd") << "\";"
                << "\"" << now.toString("hh:mm:ss") << "\";"
                << "\"" << status << "\";"
                << "\"" << operation << "\";"
                << "\"" << file << "\"\n";
        }
    }



private:
    QFile logFile;
    const int maxFiles;  // Maximum number of recent log files to retain
    LoggerType loggerType;

    /**
     * Creates a new log file with a unique name based on the current timestamp.
     * The name includes the logger type (DELETE or REPLACE) to differentiate between log types.
     * Also writes a header line to the log file for CSV formatting.
     */
    void createLogFile() {
        // Ensure the log directory exists
        if (!LOGGERS_DIR.exists()) {
            LOGGERS_DIR.mkpath(".");  // Create the loggers directory if it doesn't exist
        }

        // Create a unique file name based on the current timestamp and operation
        QString logTypeStr = (loggerType == LoggerType::DELETE) ? "delete" : "replace";
        QString logFileName = QString("%1/%2_log_%3.csv")
                                  .arg(LOGGERS_DIR.path(), logTypeStr, DateTime_Utils::currentDateTime());

        logFile.setFileName(logFileName);

        // Open the new log file and write the header with quotes around each field
        if (logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&logFile);
            out << "\"Date\";\"Time\";\"Status\";\"Operation\";\"File\"\n";
            qDebug() << "Log file created:" << logFileName;
        } else {
            qWarning() << "Could not create log file:" << logFileName;
        }
    }


    /**
     * Maintains the limit on the number of log files by deleting older log files
     * if the total number exceeds the specified maximum.
     */
    void maintainLogFileLimit() {
        QDir logDir(LOGGERS_DIR.path());

        // Set name filters based on the logger loggerType
        QString filter = (loggerType == LoggerType::DELETE) ? "delete_log_*.csv" : "replace_log_*.csv";
        logDir.setNameFilters(QStringList() << filter);  // Filter for log files

        // Sort files by creation time, keeping the newest files first
        QFileInfoList logFiles = logDir.entryInfoList(QDir::Files, QDir::Time | QDir::Reversed);

        // If there are more than `maxFiles` log files, delete the older ones
        if (logFiles.size() > maxFiles) {
            for (int i = 0; i < logFiles.size() - maxFiles; ++i) {
                if (QFile::remove(logFiles[i].absoluteFilePath()))
                    qDebug() << "Deleted old log file:" << logFiles[i].fileName();
                else
                    qWarning() << "Failed to delete old log file:" << logFiles[i].fileName();

            }
        }
    }
};
