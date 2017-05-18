#-------------------------------------------------
#
# Project created by QtCreator 2017-05-01T20:22:02
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = url_database
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    refrecord.cpp \
    urldatabase.cpp \
    tag.cpp \
    urllockchecker.cpp \
    tagslist.cpp \
    factorymethod.cpp \
    dataobject.cpp

HEADERS  += mainwindow.h \
    config.h \
    refrecord.h \
    urldatabase.h \
    tag.h \
    urllockchecker.h \
    makestring.h \
    tagslist.h \
    initdb.h \
    compositepattern.h \
    workingdb.h \
    factorymethod.h \
    dataobject.h

FORMS    += ui/mainwindow.ui \
    ui/addreferencedialog.ui

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CFLAGS_WARN_ON = -Wall

RESOURCES +=

DISTFILES += \
    diagram.qmodel
