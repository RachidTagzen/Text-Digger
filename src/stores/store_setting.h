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

#include <QAtomicInt>
#include <QString>


// *******************************************************************************************************************
// ************************************************ Static variables *************************************************
// *******************************************************************************************************************
static QAtomicInt COUNTER;


class Store_Setting {

public:

    // *******************************************************************************************************************
    // ************************************************** Constructors ***************************************************
    // *******************************************************************************************************************
    Store_Setting() {
        this->m_id = COUNTER.operator++();
        this->m_key = "";
        this->m_value = "";
        this->m_defaultValue = "";
    }


    inline Store_Setting(const QString &key, const QString &value, const QString &defaultValue)
        : m_id(COUNTER.operator++()), m_key(key), m_value(value), m_defaultValue(defaultValue) { }


    // *******************************************************************************************************************
    // ***************************************************** Getters *****************************************************
    // *******************************************************************************************************************
    int getId() const {
        return m_id;
    }

    QString getKey() const {
        return m_key;
    }

    QString getValue() const {
        return m_value;
    }

    QString getDefaultValue() const {
        return m_defaultValue;
    }


    // *******************************************************************************************************************
    // ***************************************************** Setters *****************************************************
    // *******************************************************************************************************************
    void setId(int value) {
        this->m_id = value;
    }

    void setKey(const QString &value) {
        this->m_key = value;
    }

    void setValue(const QString &value) {
        this->m_value = value;
    }

    void setDefaultValue(const QString &value) {
        m_defaultValue = value;
    }



    // *******************************************************************************************************************
    // **************************************************** Functions ****************************************************
    // *******************************************************************************************************************
    void resetCounterIncrement() {
        // counter = -1;
        COUNTER.storeRelease(-1);
    }

    const QString toString() {
        return QString("Id : %0; Key : %1; Value : %2; Default Value : %3")
        .arg(this->m_id).arg(this->m_key,
                             this->m_value,
                             this->m_defaultValue);
    }



private:
    int m_id;
    QString m_key;
    QString m_value;
    QString m_defaultValue;


};
