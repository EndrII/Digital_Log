#-------------------------------------------------
#
# Project created by QtCreator 2016-04-13T09:37:12
#
#-------------------------------------------------

QT       += core gui
CONFIG   += C++14
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Digital_Log
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Etime.cpp \
    group.cpp \
    groupvoid.cpp \
    EError.cpp \
    database.cpp \
    sourcetable.cpp \
    starastamode.cpp \
    groupmenager.cpp \
    log.cpp \
    setings.cpp \
    datewidget.cpp \
    readarchiv.cpp

HEADERS  += mainwindow.h \
    core.h \
    Etime.h \
    group.h \
    groupvoid.h \
    EError.h \
    database.h \
    sourcetable.h \
    starastamode.h \
    groupmenager.h \
    log.h \
    setings.h \
    datewidget.h \
    readarchiv.h

FORMS += \
    form.ui
