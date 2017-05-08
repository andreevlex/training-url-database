#-------------------------------------------------
#
# Project created by QtCreator 2017-05-01T20:22:02
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = url_db
TEMPLATE = app


SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/refrecord.cpp \
    src/urldatabase.cpp \
    src/tag.cpp

HEADERS  += src/mainwindow.h \
    src/config.h \
    src/refrecord.h \
    src/urldatabase.h \
    src/tag.h

FORMS    += src/ui/mainwindow.ui

QMAKE_CXXFLAGS += -std=c++11 -lGL
