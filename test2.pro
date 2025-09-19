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
    form_page3.cpp \
    chkresultclass/stream_result_all_data.cpp \
    tablegenerator/tablegenerator.cpp

HEADERS += \
        mainwindow.h \
    form_page1.h \
    form_page2.h \
    pdfgenerator/pdfgenerator.h \
    form_page3.h \
    chkresultclass/stream_result_all_data.h \
    tablegenerator/tablegenerator.h

FORMS += \
        mainwindow.ui \
    form_page1.ui \
    form_page2.ui \
    form_page3.ui
