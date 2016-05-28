#-------------------------------------------------
#
# Project created by QtCreator 2016-04-13T09:37:12
#
#-------------------------------------------------

QT       += core gui
CONFIG   += C++14
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DigitalLog
TEMPLATE = app
VERSION = 1.0.1.0
TEMPLATE = app
QMAKE_TARGET_COMPANY = Yankovich Andrei
QMAKE_TARGET_PRODUCT = DigitalLog
QMAKE_TARGET_DESCRIPTION = Digital Log
QMAKE_TARGET_COPYRIGHT = (c) Yankovich Andrei
RC_ICONS =logo.ico

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
    setings.cpp \
    datewidget.cpp \
    readarchiv.cpp \
    about.cpp \
    help.cpp

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
    setings.h \
    datewidget.h \
    readarchiv.h \
    about.h \
    help.h

FORMS +=

RESOURCES += \
    res.qrc
