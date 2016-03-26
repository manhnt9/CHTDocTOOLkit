#-------------------------------------------------
#
# Project created by QtCreator 2013-10-28T14:05:20
#
#-------------------------------------------------

QT       += core gui

QT += widgets
QT += network

TARGET = CHTDocViewer
TEMPLATE = app

INCLUDEPATH += "C:\Users\mant\Documents\Manh_12LY\CHTDocTOOLkit\include"


SOURCES += main.cpp\
        chtdocviewer.cpp \
    ../../../include/docs.cpp \
    filedownloader.cpp \
    imageviewer.cpp

HEADERS  += chtdocviewer.h \
    ../../../include/docs.h \
    filedownloader.h \
    imageviewer.h

FORMS    += chtdocviewer.ui \
    imageviewer.ui
