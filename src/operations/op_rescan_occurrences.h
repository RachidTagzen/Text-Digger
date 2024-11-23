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

#include <QPair>
#include <QElapsedTimer>
#include <QRegularExpression>
#include <QFile>


class RescanOccurrences {

public:

    static QPair<int, QSet<int>> scan(QFile &file, const bool &fileReadingTimeout, const int &timeoutFileReading,
                                      const bool &limitOccurrencesFound, const int &occurrencesFoundLimit,
                                      const QRegularExpression &searchTextPattern, const bool &cancel) {

        // Initialize the elapsed timer if timeout is enabled
        QElapsedTimer timer;
        if (fileReadingTimeout)
            timer.start();


        int lineNumber = 0;
        int occurrences = 0;
        QSet<int> linesNumbers;
        int checkInterval = 100;  // Check timeout every 100 lines
        int linesProcessed = 0;   // Counter for processed lines


        QTextStream in(&file);

        // Read the file line by line
        while (!in.atEnd()) {

            lineNumber++;

            // Check for timeout every 'checkInterval' lines if enabled
            if (fileReadingTimeout && linesProcessed >= checkInterval) {
                if (cancel)
                    return QPair<int, QSet<int>>(occurrences, linesNumbers);

                if (timer.elapsed() > timeoutFileReading * 1000) {
                    qWarning() << "File reading timeout reached for" << file.fileName();
                    break;
                }

                linesProcessed = 0;  // Reset the counter after checking the timeout
            }

            QString line = in.readLine();


            // Use QRegularExpression to search for all occurrences in the line
            QRegularExpressionMatchIterator matchIterator = searchTextPattern.globalMatchView(line);

            while (matchIterator.hasNext()) {
                matchIterator.next();
                linesNumbers.insert(lineNumber);
                occurrences++;

                // If occurrence limit is enabled and reached, stop searching
                if (limitOccurrencesFound && occurrences >= occurrencesFoundLimit) {
                    qWarning() << "Occurrences limit reached for" << file.fileName();
                    break;
                }
            }

            linesProcessed++;

            // Break out of the outer loop if the limit has been reached
            if (limitOccurrencesFound && occurrences >= occurrencesFoundLimit)
                break;

        }

        return QPair<int, QSet<int>>(occurrences, linesNumbers);
    }

};
