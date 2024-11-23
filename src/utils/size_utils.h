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


class Size_Utils {

public:

    /**
     * Converts a size in bytes to a human-readable format (e.g., KB, MB, GB).
     * @param sizeBytes - Size in bytes to convert.
     * @param system - The measurement system ("SI" for decimal or "IEC" for binary).
     * @return A QString representing the human-readable size.
     */
    static QString convertSizeToHuman(qint64 sizeBytes, const QString &system) {

        if (sizeBytes < 0)
            return "Invalid size";

        if (sizeBytes == 0)
            return "0 Bytes";

        // Get the units for the specified system, default to SI if not found
        const QStringList units = SIZE_SYSTEM_AND_UNITS.value(system, SIZE_SYSTEM_AND_UNITS["SI"]);
        int index = 0;

        // Initialize the size for calculations
        double size = static_cast<double>(sizeBytes);

        // Divide by 1024 for IEC, and by 1000 for SI (Determine the divisor based on the measurement system)
        double divisor = (system == "SI") ? 1000.0 : 1024.0;

        // Calculate the appropriate unit index by dividing the size
        while (size >= divisor && index < units.size() - 1) {
            size /= divisor; // Divide the size by the appropriate base (divisor)
            ++index; // Increment the unit index
        }

        // Format the size; check if it is a whole number for formatting
        QString formattedSize;
        if (size == static_cast<qint64>(size))
            formattedSize = QString::number(static_cast<qint64>(size)); // If size is a whole number, convert to integer
        else
            formattedSize = QString::number(size, 'f', 1); // One decimal place for fractional sizes

        // Determine the correct unit and its plural form
        QString unit = units[index];
        QString unitPlural = units[index + (size == 1.0 ? 0 : 1)]; // Use plural if size is not 1

        // Return the formatted size with the appropriate unit
        return formattedSize + " " + (size == 1.0 ? unit : unitPlural);
    }


    /**
     * Converts a size from a specified unit to bytes.
     * @param size - The size to convert.
     * @param unit - The unit of the size (e.g., "KB", "MB").
     * @param system - The measurement system ("SI" or "IEC").
     * @return The size in bytes as a qint64, or 0 if the unit is invalid.
     */
    static qint64 convertSize(double size, const QString &unit, const QString &system) {

        // Select the conversion map based on the measurement system
        const QMap<QString, double> &conversions = (system == "SI") ? SI_CONVERSIONS : IEC_CONVERSIONS;

        // Check if the unit is valid. Returns 0 for invalid units (consider exception handling if needed)
        if (!conversions.contains(unit))
            return 0;

        // Convert the size to bytes and return
        return static_cast<qint64>(size * conversions[unit]);
    }


    /**
     * Checks if a file size matches specified conditions against given size values.
     * @param fileSize - The actual size of the file.
     * @param sizeSystem - The measurement system used ("SI" or "IEC").
     * @param sizeCondition - The condition to check (e.g., "Equals", "Less than").
     * @param sizeValue1 - The first size value for comparison.
     * @param sizeValue2 - The second size value for comparison (used for "Between" and "Not between" conditions).
     * @param sizeUnit_1 - The unit of sizeValue1 (e.g., "KB").
     * @param sizeUnit_2 - The unit of sizeValue2 (e.g., "MB").
     * @return True if the file size meets the specified conditions; otherwise, false.
     */
    static bool matchesSizeConditions(const qint64 &fileSize, const QString &sizeSystem, const QString &sizeCondition,
                                      double sizeValue1, double sizeValue2,
                                      const QString &sizeUnit_1, const QString &sizeUnit_2) {

        // Convert the size conditions from the specified units to bytes
        qint64 convertedSizeValue1 = Size_Utils::convertSize(sizeValue1, sizeUnit_1, sizeSystem);
        qint64 convertedSizeValue2 = Size_Utils::convertSize(sizeValue2, sizeUnit_2, sizeSystem);

        // Evaluate the condition against the file size
        if (sizeCondition == "Equals")
            return fileSize == convertedSizeValue1;
        else if (sizeCondition == "Not equals")
            return fileSize != convertedSizeValue1;
        else if (sizeCondition == "Less than")
            return fileSize < convertedSizeValue1;
        else if (sizeCondition == "Greater than")
            return fileSize > convertedSizeValue1;
        else if (sizeCondition == "Between")
            return fileSize >= convertedSizeValue1 && fileSize <= convertedSizeValue2;
        else if (sizeCondition == "Not between")
            return fileSize < convertedSizeValue1 || fileSize > convertedSizeValue2;

        return false;
    }

};
