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

#include "QTime"


class DateTime_Utils {

public:

    /**
     * Formats a given QDateTime object based on whether it falls within the current day.
     * If the date part of the provided QDateTime matches today's date, it returns only the time in "hh:mm" format.
     * Otherwise, it returns the full date in "yyyy-MM-dd" format.
     *
     * @param dateTime - The QDateTime object to format.
     * @return QString - Formatted date and/or time as a string.
     */
    static QString formatDateTimeWithinToday(const QDateTime& dateTime) {
        QDateTime currentDateTime = QDateTime::currentDateTime();
        return dateTime.date() == currentDateTime.date() ? dateTime.toString("hh:mm") : dateTime.toString("yyyy-MM-dd");
    }


    /**
     * Converts a duration in milliseconds to a time string formatted as "hh:mm:ss".
     * This is useful for displaying elapsed time in a readable format.
     *
     * @param milliseconds - The duration to convert, in milliseconds.
     * @return QString - Formatted time as "hh:mm:ss".
     */
    static QString millisecondsToHHmmss(qint64 milliseconds) {
        QTime time(0, 0, 0);           // Initialize a QTime object at 00:00:00
        time = time.addMSecs(milliseconds); // Add the provided milliseconds to the QTime
        return time.toString("hh:mm:ss");
    }


    /**
     * Formats an elapsed time given in milliseconds as "hh:mm:ss".
     * Manually computes hours, minutes, and seconds for cases where milliseconds exceed one day.
     *
     * @param milliseconds - The duration to convert, in milliseconds.
     * @return QString - The formatted elapsed time as "hh:mm:ss", with zero-padded hours, minutes, and seconds.
     */
    static QString formatElapsedTime(qint64 milliseconds) {
        qint64 seconds = milliseconds / 1000;         // Convert to total seconds
        qint64 hours = seconds / 3600;                // Calculate total hours
        qint64 minutes = (seconds % 3600) / 60;       // Calculate remaining minutes
        qint64 secs = seconds % 60;                   // Calculate remaining seconds

        // Format as "hh:mm:ss" with zero-padding for single-digit values
        return QString("%1:%2:%3")
            .arg(hours, 2, 10, QLatin1Char('0'))      // Zero-pad hours
            .arg(minutes, 2, 10, QLatin1Char('0'))    // Zero-pad minutes
            .arg(secs, 2, 10, QLatin1Char('0'));      // Zero-pad seconds
    }


    /**
     * Checks if a QDateTime satisfies a specified date condition relative to other date values.
     * Supports a range of conditions including "Equals", "Not equals", "Before", "After", "Between", and "Not between".
     *
     * @param date - The QDateTime to evaluate.
     * @param dateCondition - Condition to apply ("Equals", "Not equals", etc.).
     * @param dateValue1 - First comparison QDateTime (used for most conditions).
     * @param dateValue2 - Second comparison QDateTime (used only for "Between" and "Not between" conditions).
     * @return bool - True if the date meets the specified condition; false otherwise.
     */
    static bool matchesDateConditions(const QDateTime &date, const QString &dateCondition, const QDateTime &dateValue1,
                                      const QDateTime &dateValue2) {

        if (dateCondition == "Equals")
            return date == dateValue1;
        else if (dateCondition == "Not equals")
            return date != dateValue1;
        else if (dateCondition == "Before")
            return date < dateValue1;
        else if (dateCondition == "After")
            return date > dateValue1;
        else if (dateCondition == "Between")
            return date >= dateValue1 && date <= dateValue2;
        else if (dateCondition == "Not between")
            return date < dateValue1 || date > dateValue2;

        return false; // Return false if an unsupported condition is provided
    }



    static QString currentDateTime(const QString &format = "yyyyMMdd_hhmmss") {
        return QDateTime::currentDateTime().toString(format);
    }

};
