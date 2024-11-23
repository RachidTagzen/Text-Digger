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

#include "appsettings.h"
#include "utils/file_utils.h"

#include <QtSql/QtSql>


/**
 * The Database_Settings class is responsible for managing the settings stored in an SQLite database.
 * It includes functionality for creating tables, loading, updating, and querying settings.
 * The settings are stored in a database file that is created if it does not exist.
 *
 * The class ensures atomicity in database operations by using transactions and handles errors through
 * detailed logging. It also provides methods for optimizing the database after updates.
 */
class Database_Settings : public QObject {
    Q_OBJECT


public:

    // *******************************************************************************************************************
    // ************************************************** Constructors ***************************************************
    // *******************************************************************************************************************
    /**
     * Constructor that initializes the database settings.
     * This constructor checks for the existence of the settings file and creates one if it doesn't exist.
     * It also sets up the SQLite database for storing settings, loading SQL queries from resource files.
     */
    Database_Settings() {

        // ---------------------------------------
        // ---------------------------------------
        m_queryCreateTable = File_Utils::readFileFromResources(":/sql/resources/sql/settings/create_table.sql");
        m_queryCheckKeyExistence = File_Utils::readFileFromResources(":/sql/resources/sql/settings/check_key_existence.sql");
        m_queryInsertDefaultKeys = File_Utils::readFileFromResources(":/sql/resources/sql/settings/insert_default_keys.sql");
        m_queryInsertIgnoreDefaultKeys = File_Utils::readFileFromResources(":/sql/resources/sql/settings/insert_ignore_default_keys.sql");
        m_queryLoadIndividual = File_Utils::readFileFromResources(":/sql/resources/sql/settings/load_individual.sql");
        m_queryLoadAll = File_Utils::readFileFromResources(":/sql/resources/sql/settings/load_all.sql");
        m_queryUpdateIndividual = File_Utils::readFileFromResources(":/sql/resources/sql/settings/update_individual.sql");
        m_queryUpdateAll = File_Utils::readFileFromResources(":/sql/resources/sql/settings/update_all.sql");

        // ---------------------------------------
        // Create a QFile object for the settings file.
        // ---------------------------------------
        QFile settingsFile(SETTINGS_FILE.absoluteFilePath());

        // ---------------------------------------
        // If the settings file does not exist, create it.
        // ---------------------------------------
        if (!SETTINGS_FILE.isFile())
            settingsFile.open(QIODevice::WriteOnly);

        // ---------------------------------------
        // Close the file after creation or checking its existence.
        // ---------------------------------------
        settingsFile.close();

        // ---------------------------------------
        // Create a QFile object for the database file.
        // ---------------------------------------
        QFile databaseFile(SETTINGS_FILE.absoluteFilePath());

        // ---------------------------------------
        // Check if a database connection with the name "Settings Database" already exists.
        // ---------------------------------------
        if (QSqlDatabase::contains("Settings Database")) {
            // If the database is open, close it.
            if (m_database.isOpen())
                m_database.close();

            // Remove the existing database connection.
            QSqlDatabase::removeDatabase("Settings Database");
        }

        // ---------------------------------------
        // Add a new SQLite database connection named "Settings Database".
        // ---------------------------------------
        m_database = QSqlDatabase::addDatabase("QSQLITE", "Settings Database");

        // ---------------------------------------
        // Set the database name to the path of the database file.
        // ---------------------------------------
        m_database.setDatabaseName(databaseFile.fileName());
    }



    // *******************************************************************************************************************
    // ************************************************* Creating Tables *************************************************
    // *******************************************************************************************************************
    /**
     * Creates the necessary tables in the database based on the provided App_Settings object.
     * Iterates over the settings and inserts them if they do not exist.
     *
     * @param appSettings A reference to the App_Settings object containing the settings to be added to the database.
     */
    void createTables(App_Settings *&appSettings) {
        // Attempt to open the database connection.
        if (!openDatabase())
            return;

        // Start a database transaction to ensure atomicity.
        m_database.transaction();

        // ---------------------------------------
        // Create Settings Tables
        // ---------------------------------------
        QSqlQuery createTable_Query(m_database);

        // Execute the query to create the settings table.
        if (!createTable_Query.exec(m_queryCreateTable)) {
            qDebug() << createTable_Query.lastQuery();
            qDebug() << createTable_Query.lastError();
            m_database.rollback();
            m_database.close();
            return;
        }

        // ---------------------------------------
        // Add settings' Keys
        // ---------------------------------------
        QSqlQuery sqlQuery_IsKeyExists(m_database);
        QSqlQuery sqlQuery(m_database);

        // Get the list of settings from the App_Settings object.
        QVector<Store_Setting> settingsList = appSettings->getSettingsList();

        // Iterate over each setting to check if it already exists in the database.
        foreach (Store_Setting storeSetting, settingsList) {
            sqlQuery_IsKeyExists.prepare(m_queryCheckKeyExistence);
            sqlQuery_IsKeyExists.bindValue(":C0", storeSetting.getKey());

            // Execute the query to check if the key exists.
            if (sqlQuery_IsKeyExists.exec() && sqlQuery_IsKeyExists.first()) {

                bool keyExists = sqlQuery_IsKeyExists.value(0).toBool();
                if (!keyExists) {
                    sqlQuery.prepare(m_queryInsertDefaultKeys);
                    sqlQuery.bindValue(":C0", storeSetting.getKey());
                    sqlQuery.bindValue(":C1", "");
                    sqlQuery.bindValue(":C2", storeSetting.getDefaultValue());
                    sqlQuery.exec();
                    sqlQuery.clear();  // Clear the query for reuse.
                }
            }

            sqlQuery_IsKeyExists.clear();  // Clear the existence check query.
        }


        // Commit the transaction if all operations are successful.
        m_database.commit();
        m_database.close();
    }



    // *******************************************************************************************************************
    // ***************************************************** Loading *****************************************************
    // *******************************************************************************************************************
    /**
     * Loads all settings from the database and returns them as a QVector of Store_Setting objects.
     *
     * @return A QVector of Store_Setting objects containing the settings loaded from the database.
     */
    QVector<Store_Setting> load_Settings() {

        QVector<Store_Setting> settingsList;

        // Attempt to open the database connection.
        if (!openDatabase())
            return settingsList;

        QSqlQuery sqlQuery(m_database);

        // Execute a query to select all settings from the database.
        if (sqlQuery.exec("SELECT * FROM settings;")) {
            settingsList.reserve(sqlQuery.size());  // Reserve space for settings.

            // Iterate through the results and populate the settings list.
            while (sqlQuery.next()) {
                QString key = sqlQuery.value("key").toString();
                QString value = sqlQuery.value("value").toString();
                QString defaultValue = sqlQuery.value("defaultvalue").toString();
                settingsList.append(Store_Setting(key, value, defaultValue));
            }

        } else {
            qDebug() << "Error executing query:" << sqlQuery.lastError().text();
        }

        m_database.close();
        return settingsList;
    }


    /**
     * Loads a single setting identified by the provided key.
     *
     * @param key The key of the setting to be loaded.
     * @return A Store_Setting object containing the loaded setting.
     */
    Store_Setting load_Setting(QString key) {
        Store_Setting storeSetting;

        // Attempt to open the database connection.
        if (!openDatabase())
            return storeSetting;

        QSqlQuery sqlQuery(m_database);
        sqlQuery.prepare(m_queryLoadIndividual);
        sqlQuery.bindValue(":C0", key);

        // Execute the query to load the setting.
        if (sqlQuery.exec()) {
            // If a record is found, populate the storeSetting object.
            if (sqlQuery.next()) {
                QSqlRecord record = sqlQuery.record();
                storeSetting.setKey(record.value("key").toString());
                storeSetting.setValue(record.value("value").toString());
                storeSetting.setDefaultValue(record.value("defaultvalue").toString());
            }

        } else {
            qDebug() << "Error executing query:" << sqlQuery.lastError().text();
        }

        m_database.close();
        return storeSetting;
    }



    // *******************************************************************************************************************
    // **************************************************** Updating *****************************************************
    // *******************************************************************************************************************
    /**
     * Updates all settings in the database based on the provided App_Settings object.
     *
     * @param appSettings A reference to the App_Settings object containing the settings to be updated in the database.
     */
    void update_Settings(App_Settings *&appSettings) {

        if (!openDatabase())
            return;

        m_database.transaction();

        QSqlQuery sqlQuery_Update(m_database);
        sqlQuery_Update.prepare(m_queryUpdateAll);

        // Retrieve the list of settings from the App_Settings object.
        const QVector<Store_Setting> &settingsList = appSettings->getSettingsList();

        // Iterate over each setting and update it in the database.
        foreach (Store_Setting storeSetting, settingsList) {
            sqlQuery_Update.bindValue(":C0", storeSetting.getKey());
            sqlQuery_Update.bindValue(":C1", storeSetting.getValue());
            sqlQuery_Update.bindValue(":C2", storeSetting.getDefaultValue());
            sqlQuery_Update.bindValue(":C3", storeSetting.getKey());
            // qDebug() << storeSetting.getKey() << "  -   " << storeSetting.getValue();

            if (!sqlQuery_Update.exec()) {
                qDebug() << "Error updating query:" << sqlQuery_Update.lastError().text();
                qDebug() << "Last query executed:" << sqlQuery_Update.lastQuery();
            }

            // sqlQuery_Update.clear();
        }

        m_database.commit();
        vacuum();
        m_database.close();

        qDebug() << "Settings are saved to database.";
    }


    /**
     * Updates or inserts a specific setting identified by the key.
     * If the setting exists, it is updated; otherwise, it is inserted.
     *
     * @param key The key of the setting to be updated or inserted.
     * @param value The new value for the setting.
     * @param defaultValue The default value for the setting.
     */
    void update_Setting(QString key, QString value, QString defaultValue) {

        if (!openDatabase())
            return;

        QSqlQuery sqlQuery(m_database);
        sqlQuery.prepare(m_queryCheckKeyExistence);
        sqlQuery.bindValue(":C0", key);

        if (sqlQuery.exec() && sqlQuery.first()) {
            bool keyExists = sqlQuery.value(0).toBool();
            sqlQuery.clear();

            // If the key exists, prepare the update query; otherwise, prepare the insert query.
            if (keyExists) {
                sqlQuery.prepare(m_queryUpdateIndividual);
                sqlQuery.bindValue(":C0", key);
                sqlQuery.bindValue(":C1", value);
            } else {
                sqlQuery.prepare(m_queryInsertDefaultKeys);
                sqlQuery.bindValue(":C0", key);
                sqlQuery.bindValue(":C1", value);
                sqlQuery.bindValue(":C2", defaultValue);
            }

            if (!sqlQuery.exec()) {
                qDebug() << "Error updating or inserting query:" << sqlQuery.lastError().text();
                qDebug() << "Last query executed:" << sqlQuery.lastQuery();
            }

        } else {
            qDebug() << "Error checking key existence:" << sqlQuery.lastError().text();
            qDebug() << "Last query executed:" << sqlQuery.lastQuery();
        }

        vacuum();
        m_database.close();
    }




private:
    QSqlDatabase m_database;

    QString m_queryCreateTable;
    QString m_queryCheckKeyExistence;
    QString m_queryInsertDefaultKeys;
    QString m_queryInsertIgnoreDefaultKeys;
    QString m_queryLoadIndividual;
    QString m_queryLoadAll;
    QString m_queryUpdateIndividual;
    QString m_queryUpdateAll;


    /**
     * Opens the database connection.
     *
     * @return True if the database connection is successfully opened, otherwise false.
     */
    bool openDatabase() {
        if (!m_database.open()) {
            qDebug() << "Settings Database not opened!";
            return false;  // Return false if the database cannot be opened.
        }
        return true;  // Return true if the database is successfully opened.
    }


    /**
     * Optimizes the database by reclaiming unused space.
     */
    void vacuum() {
        QSqlQuery query_Vacuum(m_database);
        query_Vacuum.prepare("VACUUM");
        query_Vacuum.exec();
    }


};
