#-------------------------------------------------
#
# Project created by QtCreator 2013-10-25T14:21:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WebmasterTOOL
TEMPLATE = app

INCLUDEPATH += "C:\Users\mant\Documents\Manh_12LY\CHTDocTOOLkit\include"

SOURCES += main.cpp\
        webmastertool.cpp \
    ../../../include/docs.cpp \
    imageviewer.cpp \
    uploaddialog.cpp


HEADERS  += webmastertool.h \
    ../../../include/docs.h \
    imageviewer.h \
    uploaddialog.h


FORMS    += webmastertool.ui \
    imageviewer.ui \
    uploaddialog.ui

RESOURCES += \
    icons.qrc
