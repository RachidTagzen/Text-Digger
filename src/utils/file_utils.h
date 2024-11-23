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


class File_Utils {

public:

    /**
     * Creates necessary directories and files by invoking helper functions.
     * This method creates the `SETTINGS_DIR` and `LOGGERS_DIR` directories
     * and the `SETTINGS_FILE`.
     */
    static void createNecessaryDirectoriesAndFiles(bool logIfExists = true) {
        createDirectories({ SETTINGS_DIR, LOGGERS_DIR }, logIfExists);
        createFile(SETTINGS_FILE, logIfExists);
    }

    /**
 * Creates a directory at the specified path if it does not already exist.
 * @param dirPath - The path of the directory to be created.
 * @param logIfExists - Whether to log a message if the directory already exists.
 * @return True if the directory already exists or was created successfully, false if creation failed.
 */
    static bool createDirectory(const QString &dirPath, bool logIfExists = true) {
        QDir dir;

        if (dir.exists(dirPath)) {
            if (logIfExists)
                qDebug() << "Directory already exists : " << dirPath;

            return true;  // Directory exists, no creation needed, return true
        }

        bool success = dir.mkpath(dirPath);
        if (!success) {
            qWarning() << "Failed to create directory : " << dirPath;
            return false;  // Directory creation failed
        }

        return true;  // Directory was created successfully
    }


    /**
     * Overloaded method to create a directory using a QDir object.
     * @param dir - QDir object representing the directory path.
     * @return True if the directory exists or was successfully created.
     */
    static bool createDirectory(const QDir &dir, bool logIfExists = true) {
        return createDirectory(dir.absolutePath(), logIfExists);
    }

    /**
     * Creates multiple directories specified in a QVector.
     * @param dirs - QVector of QDir objects representing directory paths.
     */
    static void createDirectories(const QVector<QDir> &dirs, bool logIfExists = true) {
        for (const QDir &dir : dirs)
            createDirectory(dir, logIfExists);
    }


    /**
 * Creates a file at the specified path if it does not already exist.
 * @param filePath - The path of the file to be created.
 * @param logIfExists - Whether to log a message if the file already exists.
 * @return True if the file already exists or was created successfully, false if creation failed.
 */
    static bool createFile(const QString &filePath, bool logIfExists) {
        if (QFile::exists(filePath)) {
            if (logIfExists)
                qWarning() << "File already exists : " << filePath;

            return true;  // File exists, no creation needed, return true
        }

        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::NewOnly)) {
            qWarning() << "Failed to create file : " << filePath;
            return false;  // File creation failed
        }

        file.close();
        return true;  // File created successfully
    }


    /**
     * Overloaded method to create a file using a QFile object.
     * @param file - QFile object representing the file.
     * @return True if the file was created successfully, false otherwise.
     */
    static bool createFile(QFile &file, bool logIfExists = true) {
        return createFile(file.fileName(), logIfExists);
    }


    /**
     * Overloaded method to create a file using a QFileInfo object.
     * @param fileInfo - QFileInfo object containing file path information.
     * @return True if the file was created successfully, false otherwise.
     */
    static bool createFile(const QFileInfo &fileInfo, bool logIfExists = true) {
        return createFile(fileInfo.absoluteFilePath(), logIfExists);
    }


    /**
     * Creates a file within a specified directory. The directory is created first if needed.
     * @param dirPath - Path to the directory where the file should be created.
     * @param fileName - Name of the file to create within the directory.
     * @return True if the file was created successfully, false otherwise.
     */
    static bool createFileInDirectory(const QString &dirPath, const QString &fileName, bool logIfExists = true) {
        if (!createDirectory(dirPath)) {
            qWarning() << "Failed to create directory : " << dirPath;
            return false;
        }

        QString filePath = QDir(dirPath).filePath(fileName);
        return createFile(filePath, logIfExists);
    }


    /**
     * Reads and returns the entire content of a file from resources as a QString.
     * @param resourcePath - Path to the resource file.
     * @return File content as QString; returns an empty string if file open fails.
     */
    static QString readFileFromResources(const QString& resourcePath) {
        QFile file(resourcePath);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Failed to open file:" << resourcePath;
            return ""; // Return an empty string or handle the error as needed
        }

        // Read the entire content of the file
        QTextStream in(&file);
        QString content = in.readAll();

        // No need to explicitly close the file; it will be closed automatically
        return content;
    }

    /**
     * Reads lines from a specified file, with options to avoid duplicates and sort.
     * @param filePath - Path to the file to read.
     * @param avoidDuplicates - If true, removes duplicate lines.
     * @param sort - If true, sorts lines alphabetically.
     * @return QStringList of lines read from the file.
     */
    QStringList readLinesFromFile(const QString &filePath, bool avoidDuplicates, bool sort) {
        QFile file(filePath);
        return readLinesFromFile(file, avoidDuplicates, sort);
    }

    /**
     * Reads lines from an open QFile object, with options to avoid duplicates and sort.
     * @param file - QFile object to read from.
     * @param avoidDuplicates - If true, removes duplicate lines.
     * @param sort - If true, sorts lines alphabetically.
     * @return QStringList of lines read from the file.
     */
    QStringList readLinesFromFile(QFile &file, bool avoidDuplicates, bool sort) {

        QStringList lines;

        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream.setEncoding(QStringConverter::Utf8);
            stream.setGenerateByteOrderMark(false);

            while (!stream.atEnd()) {
                QString line = stream.readLine().trimmed();

                if (!line.isEmpty())
                    lines.append(line);
            }

            file.close();
        }

        if (avoidDuplicates)
            lines.removeDuplicates();

        if (sort)
            lines.sort(Qt::CaseInsensitive);

        return lines;
    }

    /**
     * Writes a QStringList of lines to a file with options to remove duplicates and sort.
     * @param filePath - Path of the file to write.
     * @param list - List of lines to write.
     * @param avoidDuplicates - If true, removes duplicate lines before writing.
     * @param sort - If true, sorts lines before writing.
     */
    void writeToFile(const QString &filePath, QStringList list, const bool avoidDuplicates, const bool sort) {
        QFile file(filePath);
        writeToFile(file, list, avoidDuplicates, sort);
    }

    /**
     * Writes a QStringList of lines to an open QFile object, with options for duplicates and sorting.
     * @param file - QFile object to write to.
     * @param list - List of lines to write.
     * @param avoidDuplicates - If true, removes duplicate lines before writing.
     * @param sort - If true, sorts lines before writing.
     */
    void writeToFile(QFile &file, QStringList list, const bool avoidDuplicates, const bool sort) {

        if (avoidDuplicates)
            list.removeDuplicates();

        if (sort)
            list.sort(Qt::CaseInsensitive);

        writeToFile(file, list.join("\n"));
    }

    /**
     * Writes a single string of text to a file.
     * @param filePath - Path of the file to write.
     * @param text - Text to write to the file.
     */
    void writeToFile(const QString &filePath, const QString text) {
        QFile file(filePath);
        writeToFile(file, text);
    }

    /**
     * Writes a string of text to an open QFile object, replacing any existing content.
     * @param file - QFile object to write to.
     * @param text - Text to write to the file.
     */
    void writeToFile(QFile &file, const QString text) {

        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
            QTextStream outputStream(&file);
            outputStream.setEncoding(QStringConverter::Utf8);
            outputStream.setGenerateByteOrderMark(false);
            outputStream << text;
            file.flush();
            file.close();
        }
    }

    /**
     * Checks if the given QByteArray matches any known binary file signatures.
     * @param data - QByteArray to check.
     * @return True if a known binary signature is found, false otherwise.
     */
    static bool isKnownBinaryFile(const QByteArray &data) {
        // Iterate over the set and check if the file starts with any of the magic numbers
        for (const QByteArray &magicNumber : binariesMagicNumbers_Set)
            if (data.startsWith(magicNumber))
                return true;

        return false;
    }

    /**
     * Checks if a file contains text (as opposed to binary data).
     * @param file - QFile object to check.
     * @return True if the file is likely a text file, false otherwise.
     */
    static bool isTextFile(QFile &file) {
        if (!file.isOpen() || !file.isReadable()) {
            qWarning() << "File is not open or readable";
            return false;
        }

        // Read the first 1024 bytes
        QByteArray data = file.peek(1024);

        // Check for known binary signatures or invalid UTF-8 sequences
        if (isKnownBinaryFile(data) || data.isEmpty() || QString::fromUtf8(data).toUtf8() != data)
            return false;

        // Check if the content contains only printable characters (or valid control characters like newline)
        QString content = QString::fromUtf8(data);
        for (QChar c : content)
            if (!c.isPrint() && c != '\n' && c != '\r' && c != '\t')
                return false;

        return true;
    }



private:
    /**
     * A set of known binary file "magic numbers" for various file types.
     * This set is used to help identify binary files by checking their first bytes.
     */
    static const inline QSet<QByteArray> binariesMagicNumbers_Set {
        // Images
        QByteArray::fromHex("FFD8FF"),               // JPEG
        QByteArray::fromHex("89504E470D0A1A0A"),     // PNG
        QByteArray::fromHex("474946383761"),         // GIF87a
        QByteArray::fromHex("474946383961"),         // GIF89a
        QByteArray::fromHex("424D"),                 // BMP
        QByteArray::fromHex("49492A00"),             // TIFF (little-endian)
        QByteArray::fromHex("4D4D002A"),             // TIFF (big-endian)
        QByteArray::fromHex("52494646"),             // WebP, WAV, AVI (further bytes needed)
        QByteArray::fromHex("00000100"),             // ICO (Windows Icon)
        QByteArray::fromHex("000100004A46494600"),   // JFIF (JPEG File Interchange Format)

        // Documents
        QByteArray::fromHex("25504446"),             // PDF
        QByteArray::fromHex("D0CF11E0A1B11AE1"),     // DOC, XLS, PPT (pre-2003, OLE Compound)
        QByteArray::fromHex("504B0304"),             // DOCX, XLSX, PPTX, JAR, APK, ZIP, ODT, EPUB (ZIP-based)

        // Archives and Compressed Files
        QByteArray::fromHex("526172211A0700"),       // RAR (v1.5 and later)
        QByteArray::fromHex("377ABCAF271C"),         // 7-Zip
        QByteArray::fromHex("1F8B"),                 // GZIP
        QByteArray::fromHex("425A68"),               // BZip2
        QByteArray::fromHex("7573746172"),           // TAR
        QByteArray::fromHex("EDABEEDB"),             // RPM
        QByteArray::fromHex("213C617263683E"),       // Debian package (DEB)
        QByteArray::fromHex("FD377A585A00"),         // XZ
        QByteArray::fromHex("5D000080"),             // LZMA
        QByteArray::fromHex("213C617263683E"),       // AR archive
        QByteArray::fromHex("303730373037"),         // CPIO archive

        // Executables and Libraries
        QByteArray::fromHex("4D5A"),                 // EXE (Windows executable)
        QByteArray::fromHex("7F454C46"),             // ELF (Unix/Linux Executable) none, .axf, .bin, .elf, .o, .out, .prx, .puff, .ko, .mod, .so
        QByteArray::fromHex("CAFEBABE"),             // Java Class file
        QByteArray::fromHex("FEEDFACE"),             // Mach-O (MacOS executable, 32-bit)
        QByteArray::fromHex("FEEDFACF"),             // Mach-O (MacOS executable, 64-bit)
        QByteArray::fromHex("CAFEBABE"),             // Mach-O Fat Binary
        QByteArray::fromHex("D0CF11E0A1B11AE1"),     // MSI (Microsoft Installer)

        // Audio/Video
        QByteArray::fromHex("494433"),               // MP3 (ID3v2)
        // QByteArray::fromHex("000000146674797071742020"), // MOV (QuickTime)
        // QByteArray::fromHex("00000020667479704D3441"), // M4A audio file
        QByteArray::fromHex("00000018667479706D70"), // MP4 video file
        QByteArray::fromHex("00000020667479706973"), // MP4 video file
        QByteArray::fromHex("667479704D534E56"), // MP4 video file
        QByteArray::fromHex("6674797069736F6D"), // MP4 video file
        QByteArray::fromHex("000001BA"),             // MPEG video file
        QByteArray::fromHex("000001B3"),             // MPEG Video
        QByteArray::fromHex("464C5601"),             // FLV video
        QByteArray::fromHex("3026B2758E66CF11"),     // ASF (WMV, WMA)
        // QByteArray::fromHex("52494646xxxx57415645"), // WAV (part of RIFF format, similar to WebP and AVI)
        QByteArray::fromHex("4F676753"),             // OGG audio file
        QByteArray::fromHex("1A45DFA3"),             // MKV video file (mkv, mka, mks, mk3d, webm)
        // QByteArray::fromHex("4D546864"),             // MIDI music file
        // QByteArray::fromHex("2E524D46"),             // RealMedia file
        // QByteArray::fromHex("2E736E64"),             // AU audio file

        // Disk Images
        QByteArray::fromHex("4344303031"),           // ISO CD/DVD image
        QByteArray::fromHex("7801730D626260"),       // DMG (Apple Disk Image)
        QByteArray::fromHex("4B444D"),               // VMDK (VMware Disk Image)

        //     // Database
        QByteArray::fromHex("53514C69746520666F726D6174203300"),  // SQLite DB

        //     // Others
        //     QByteArray::fromHex("2142444E"),             // PST (Personal Storage Table, Microsoft Outlook)
    };

};
