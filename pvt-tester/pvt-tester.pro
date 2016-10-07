#-------------------------------------------------
#
# Project created by QtCreator 2016-10-01T15:47:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pvt-tester
TEMPLATE = app

INCLUDEPATH += ./ ../include \
/home/lrleon/Aleph-w

LIBS += -L$$PWD/../lib

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS_DEBUG += -O0 -g
QMAKE_CXXFLAGS_RELEASE += -Ofast -D__extern_always_inline=inline

CONFIG += c++14

QMAKE_CXXFLAGS += -std=c++14

LIBS += $$PWD/../lib/libpvt.a -L/home/lrleon/Aleph-w -lAleph
