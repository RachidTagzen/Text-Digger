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

#include "enumerators/enums.h"
#include "hash/murmurhash3.h"

#include <QFile>
#include <QCryptographicHash>
#include <QByteArray>
#include <QDebug>


class ChecksumUtils {

public:

    // Enumeration for types of MurmurHash3 algorithms supported
    enum MurmurHash3Type {
        MURMUR_X86_32,
        MURMUR_X86_128,
        MURMUR_X64_128
    };


    // *******************************************************************************************************************
    // ******************************************* QByteArray Hash Calculation *******************************************
    // *******************************************************************************************************************

    /**
     * Calculates a hash of the provided QByteArray data using the specified QCryptographicHash algorithm.
     * @param hashAlgorithm - The cryptographic hash algorithm to use (e.g., SHA256).
     * @param data - The data to hash.
     * @return - The calculated hash in hexadecimal string format.
     */
    static QString calculateHash(QCryptographicHash::Algorithm hashAlgorithm, const QByteArray &data) {
        QCryptographicHash hash(hashAlgorithm);
        hash.addData(data);
        return QString::fromUtf8(hash.result().toHex());
    }



    // *******************************************************************************************************************
    // ********************************************** File Hash Calculation **********************************************
    // *******************************************************************************************************************
    /**
     * Calculates a hash of the contents of a file using the specified QCryptographicHash algorithm.
     * Reads the file in chunks to handle large files efficiently.
     * @param hashAlgorithm - The cryptographic hash algorithm to use.
     * @param filePath - Path to the file to hash.
     * @return - The calculated hash in hexadecimal string format, or an empty string if file could not be read.
     */
    static QString calculateHash(QCryptographicHash::Algorithm hashAlgorithm, const QString &filePath) {

        QFile file(filePath);

        if (!file.open(QIODevice::ReadOnly))
            return QByteArray();

        QCryptographicHash hash(hashAlgorithm);

        while (!file.atEnd()) {
            QByteArray data = file.read(2048); // Reads 2KB chunks
            hash.addData(data);
        }

        file.close();
        return QString::fromUtf8(hash.result().toHex());
    }


    /**
     * Calculates a MurmurHash3 hash of a file based on the specified type.
     * @param filePath - Path to the file to hash.
     * @param hashType - Type of MurmurHash3 to use (MURMUR_X86_32, MURMUR_X86_128, MURMUR_X64_128).
     * @param closeFile - Flag to close the file after reading.
     * @return - The calculated MurmurHash3 hash in hexadecimal string format.
     */
    static QString calculateMurmurHash3(const QString &filePath, MurmurHash3Type hashType, const bool closeFile) {
        QFile file(filePath);
        return calculateMurmurHash3(file, hashType, closeFile);
    }

    /**
     * Calculates a MurmurHash3 hash of an open QFile object in chunks.
     * Supports three MurmurHash3 variants.
     * @param file - Reference to an open QFile object.
     * @param hashType - Type of MurmurHash3 to use.
     * @param closeFile - Flag to close the file after reading.
     * @return - The calculated hash in hexadecimal string format.
     */
    static QString calculateMurmurHash3(QFile &file, MurmurHash3Type hashType, const bool closeFile) {

        const int chunkSize = 1024 * 1024; // 1MB chunks
        QByteArray chunk;
        QString hashStr;

        if (!file.isOpen() && !file.open(QIODevice::ReadOnly)) {
            qWarning() << "Failed to open file!";
            if (closeFile) file.close();
            return QString("");
        }

        // Ensure the file is readable
        if (!file.isReadable()) {
            qWarning() << "File is not readable";
            if (closeFile) file.close();
            return QString("");
        }

        // Calculate hash based on the specified MurmurHash3 type
        switch (hashType) {
        case MURMUR_X86_32: {
            uint32_t hash = 0; // Initial seed

            while (!(chunk = file.read(chunkSize)).isEmpty())
                MurmurHash3::MurmurHash3_x86_32(chunk.constData(), chunk.size(), hash, &hash);

            hashStr = QString::number(hash, 16).rightJustified(8, '0'); // Ensure 8 hex digits
            break;
        }
        case MURMUR_X86_128: {
            uint32_t hash[4] = {0}; // Initial seed

            while (!(chunk = file.read(chunkSize)).isEmpty())
                MurmurHash3::MurmurHash3_x86_128(chunk.constData(), chunk.size(), 0, hash);

            hashStr = QString("%1%2%3%4")
                          .arg(QString::number(hash[0], 16).rightJustified(8, '0'),
                               QString::number(hash[1], 16).rightJustified(8, '0'),
                               QString::number(hash[2], 16).rightJustified(8, '0'),
                               QString::number(hash[3], 16).rightJustified(8, '0'));

            break;
        }
        case MURMUR_X64_128: {
            uint64_t hash[2] = {0}; // Initial seed

            while (!(chunk = file.read(chunkSize)).isEmpty())
                MurmurHash3::MurmurHash3_x64_128(chunk.constData(), chunk.size(), 0, hash);

            hashStr = QString("%1%2")
                          .arg(QString::number(hash[0], 16).rightJustified(16, '0'),
                               QString::number(hash[1], 16).rightJustified(16, '0'));

            break;
        }
        default:
            qWarning() << "Invalid hash type";
            if (closeFile)
                file.close();

            return QString("");
        }

        if (closeFile) {
            file.close(); // Close the file once after processing
        }

        return hashStr;
    }




    // *******************************************************************************************************************
    // ******************************************** QString Hash Calculation *********************************************
    // *******************************************************************************************************************
    /**
     * Generates a MurmurHash3 hash for a given content string, supporting directory and MIME type hashing.
     * @param content - The content to hash.
     * @param hashContentType - The content type (DirectoryPath or MimeType).
     * @return - The hash in hexadecimal string format.
     */
    static QString generateMurmurHash(const QString &content, const HashContentType &hashContentType) {

        const QByteArray data = content.toUtf8();

        if (hashContentType == DirectoryPath) {
            // For DirectoryPath, use the x64 128-bit MurmurHash3
            uint64_t hash[2];
            // MurmurHash3::MurmurHash3_x64_128(data, std::strlen(data), 0, hash);
            MurmurHash3::MurmurHash3_x64_128(data.data(), data.size(), 0, hash);
            return QString::number(hash[0], 16) + QString::number(hash[1], 16);  // Combine both parts of 128-bit hash
        } else if (hashContentType == MimeType) {
            // For MimeType, use the x86 32-bit MurmurHash3
            uint32_t hash;
            MurmurHash3::MurmurHash3_x86_32(data.data(), data.size(), 0, &hash);
            return QString::number(hash, 16);
        }

        return "";
    }

    /**
     * Implements the djb2 hashing algorithm for std::string.
     * @param str - The string to hash.
     * @return - The hash value as an unsigned long.
     */
    static unsigned long stringHash_djb2(const std::string &str) {
        unsigned long hash = 5381;
        for (char c : str)
            hash = ((hash << 5) + hash) + c;

        return hash;
    }

    /**
     * Implements the djb2 hashing algorithm for QString.
     * @param str - The QString to hash.
     * @return - The hash value as an unsigned long.
     */
    static unsigned long stringHash_djb2(const QString &str) {
        unsigned long hash = 5381;
        for (int i = 0; i < str.length(); i++) {
            QChar c = str.at(i);
            hash = ((hash << 5) + hash) + c.unicode();
        }
        return hash;
    }

    /**
     * Generates a SHA256 hash for the provided QString.
     * @param str - The QString to hash.
     * @return - The SHA256 hash in hexadecimal string format.
     */
    static QString stringHash_SHA256(const QString &str) {
        QByteArray hashArray = QCryptographicHash::hash(str.toUtf8(), QCryptographicHash::Sha256);
        return hashArray.toHex();
    }

    /**
     * Generates a SHA512 hash for the provided QString.
     * @param str - The QString to hash.
     * @return - The SHA512 hash in hexadecimal string format.
     */
    static QString stringHash_SHA512(const QString &str) {
        QByteArray hashArray = QCryptographicHash::hash(str.toUtf8(), QCryptographicHash::Sha512);
        return hashArray.toHex();
    }

    /**
     * Generates a MurmurHash3 128-bit hash for the provided QString using x64 variant.
     * @param str - The QString to hash.
     * @return - The MurmurHash3 hash in hexadecimal string format.
     */
    static QString stringHash_MurmurHash3(const QString &str) {
        uint64_t hash[2];
        QByteArray data = str.toUtf8();
        MurmurHash3::MurmurHash3_x64_128(data.constData(), data.size(), 0, hash);
        return QString::number(hash[0], 16) + QString::number(hash[1], 16);
    }

    /**
     * Standard library hash function for std::string.
     * @param str - The string to hash.
     * @return - The hash value as a size_t.
     */
    static size_t stringHash_std(const std::string &str) {
        std::hash<std::string> hasher;
        return hasher(str);
    }

    /**
     * Standard library hash function for QString.
     * @param str - The QString to hash.
     * @return - The hash value as a size_t.
     */
    static size_t stringHash_std(const QString &str) {
        std::string stdStr = str.toStdString();
        std::hash<std::string> hasher;
        return hasher(stdStr);
    }

};
