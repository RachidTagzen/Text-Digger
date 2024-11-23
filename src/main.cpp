#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[]) {

    QApplication application(argc, argv);

    MainWindow window;
    window.setWindowTitle(QString("%1 v%2").arg(APP_TITLE, APP_VERSION));

    QCoreApplication::setOrganizationName(APP_TITLE);
    QCoreApplication::setApplicationName(APP_TITLE);
    QCoreApplication::setApplicationVersion(QString("v%1").arg(APP_VERSION));

    window.show();

    return application.exec();
}




