QT       += core gui sql svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20 qtquickcompiler

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    mainwindow.h \
    aboutwindow.h \
    statisticswindow.h \
    settingswindow.h \
    appsettings.h \
    components/filterwidget.h \
    components/replacement_dialog.h \
    components/scrollable_messagebox.h \
    components/statusbarwidget.h \
    constants/constants.h \
    constants/resources.h \
    databases/database_settings.h \
    delegates/browsable_cell_delegate.h \
    delegates/checkbox_item_delegate.h \
    enumerators/enums.h \
    hash/checksum_utils.h \
    hash/murmurhash3.h \
    models/results_model.h \
    models/results_sortfilterproxymodel.h \
    models/standardmodel.h \
    models/statisticsmodel.h \
    operations/op_copy_files.h \
    operations/op_delete_files.h \
    operations/op_find_occurrences.h \
    operations/op_handle_results.h \
    operations/op_open_files.h \
    operations/op_replace_ocurrences.h \
    operations/op_rescan_occurrences.h \
    stores/store_setting.h \
    stores/store_statistic.h \
    utils/center_utils.h \
    utils/clipboard_utils.h \
    utils/datetime_utils.h \
    utils/directories_utils.h \
    utils/file_utils.h \
    utils/logger_utils.h \
    utils/mimetypes_utils.h \
    utils/size_utils.h


SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingswindow.cpp \
    statisticswindow.cpp \
    aboutwindow.cpp \
    components/filterwidget.cpp \
    operations/op_find_occurrences.cpp


FORMS += \
    mainwindow.ui \
    settingswindow.ui \
    statisticswindow.ui \
    aboutwindow.ui


RESOURCES += \
    resources.qrc


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

