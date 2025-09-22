#-------------------------------------------------
#
# Project created by QtCreator 2025-09-16T18:43:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    QT += printsupport
}

# 添加log4qt框架，适配Qt 5.9.1

include($$PWD/log4qt/src/log4qt/log4qt.pri)

# Qt 5.9.1兼容性设置
lessThan(QT_MAJOR_VERSION, 6):lessThan(QT_MINOR_VERSION, 12) {
    DEFINES += LOG4QT_NO_QML_SUPPORT
    DEFINES += LOG4QT_NO_DATABASE_APPENDER
}

TARGET = test2
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    form_page1.cpp \
    form_page2.cpp \
    pdfgenerator/pdfgenerator.cpp \
    pdfgenerator/pdfgenerator_fix.cpp \
    form_page3.cpp \
    chkresultclass/chk_singleitem_result.cpp \
    chkresultclass/chk_items_result.cpp \
    tablegenerator/tablegenerator.cpp

HEADERS += \
        mainwindow.h \
    form_page1.h \
    form_page2.h \
    pdfgenerator/pdfgenerator.h \
    form_page3.h \
    chkresultclass/chk_singleitem_result.h \
    chkresultclass/chk_items_result.h \
    tablegenerator/tablegenerator.h

FORMS += \
        mainwindow.ui \
    form_page1.ui \
    form_page2.ui \
    form_page3.ui
