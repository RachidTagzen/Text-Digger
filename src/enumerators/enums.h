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


// Enumeration for different types of logging actions.
enum class LoggerType {
    DELETE,
    REPLACE
};


// Enumeration for types of hash content, typically used for file or directory management.
enum HashContentType {
    DirectoryPath,
    MimeType
};


// Enumeration to define different size comparison operations.
enum class SizeType {
    EQUALS,
    LESS_THAN,
    GREATER_THAN,
    BETWEEN,
    LESS_THAN__GREATER_THAN
};


// Enumeration for different units of size measurement.
enum class SizeUnit {
    B,
    KB,
    MB,
    GB
};


// Enumeration for different types of date and time comparisons.
enum class DateTimeType {
    EQUALS,
    BEFORE,
    BETWEEN,
    AFTER
};


// Enumeration for various selection states in UI elements.
enum class SelectionType {
    ALL,
    SELECTED,
    DESELECTED,
    CHECKED,
    UNCHECKED,
    NONE,
    INVERT
};


// Enumeration for different types of checking actions on selections.
enum class CheckingType {
    SELECT_ALL,
    DESELECT_ALL,
    SELECT_CHECKED,
    DESELECT_CHECKED,
    INVERT_SELECTION,

    CHECK_ALL,
    UNCHECK_ALL,
    CHECK_SELECTED,
    UNCHECK_SELECTED,
    INVERT_CHECKED
};


// Enumeration for different types of Tableviews in a user interface.
enum class TableviewType {
    RESULTS,
    INCLUDED_DIRECTORIES,
    EXCLUDED_DIRECTORIES,
    MIMETYPES,
    NONE
};


// Enumeration for different types of Icons.
enum IconType {
    SELECTED,
    UNSELECTED,
    INVERT,
    CHECKED,
    UNCHECKED,
    CANCEL,
    SEARCH,
    SELECTIONS_CHECKBOXES,
    ALL,
    COPY,
    TRASH,
    OPENFILES,
    EDITFILE
};


// Enumeration for different types of Styles.
enum StyleType {
    GLOBAL,
    CONTEXTMENUS
};


// Enumeration for different types of Strings.
enum StringType {
    TOOLTIP_FINDTEXT,
    TOOLTIP_FILENAMES,
    TOOLTIP_SIZESYSTEM,
    TOOLTIP_SIZEUNITS,
    HOMEPAGE,
    FEEDBACK,
    MIMETYPES
};
