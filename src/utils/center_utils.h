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

#include <QGuiApplication>
#include <QMainWindow>
#include <QScreen>
#include <QStyleHints>


/**
 * @brief Centers the given QMainWindow on the host or screen.
 *
 * This function centers a QMainWindow relative to a host QMainWindow if provided,
 * or on the primary screen if no host QMainWindow is specified.
 *
 * @param mainwindow QMainWindow to be centered.
 * @param host Optional QMainWindow host to center the mainwindow relative to.
 *        If nullptr, the mainwindow is centered on the screen.
 */
inline void CenterMainWindow(QMainWindow *mainwindow, QMainWindow *host = nullptr) {

    if (!mainwindow)
        return;

    if (!host) {
        // No host provided, center on the primary screen
        const QRect screenGeometry = QGuiApplication::primaryScreen()->availableGeometry();
        int coordinateX = (screenGeometry.width() - mainwindow->width()) / 2;
        int coordinateY = (screenGeometry.height() - mainwindow->height()) / 2;
        mainwindow->move(coordinateX, coordinateY);

    } else {
        // Center relative to the host
        const QRect hostRect = host->geometry();
        int coordinateX = hostRect.center().x() - mainwindow->rect().center().x();
        int coordinateY = hostRect.center().y() - mainwindow->rect().center().y();
        mainwindow->move(coordinateX, coordinateY);
    }
}


/**
 * @brief Centers the given widget on the host or screen.
 *
 * This function centers a QWidget relative to a host widget if provided,
 * or on the primary screen if no host widget is specified.
 *
 * @param widget QWidget to be centered.
 * @param host Optional QWidget host to center the widget relative to.
 *        If nullptr, the widget is centered on the screen.
 */
inline void CenterWidget(QWidget *widget, QWidget *host = nullptr) {

    if (!widget)
        return;

    // Determine the host; if not provided, fallback to the parent widget
    host = host ? host : widget->parentWidget();

    if (host) {
        // Center relative to the host's geometry
        widget->move(host->geometry().center() - widget->rect().center());

    } else {
        // Center on the screen if no host is provided
        const QRect geometry = QGuiApplication::primaryScreen()->availableGeometry();

        if (!QGuiApplication::styleHints()->showIsFullScreen()) {
            const int coordinateX = (geometry.width() - widget->width()) / 2;
            const int coordinateY = (geometry.height() - widget->height()) / 2;
            widget->move(coordinateX, coordinateY);
        }
    }
}
