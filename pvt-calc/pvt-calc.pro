#-------------------------------------------------
#
# Project created by QtCreator 2016-10-01T15:47:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pvt-calc
TEMPLATE = app

INCLUDEPATH += ./ ../include \
/home/lrleon/Aleph-w

LIBS += -L$$PWD/../lib

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
