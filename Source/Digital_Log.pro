#-------------------------------------------------
#
# Project created by QtCreator 2016-04-13T09:37:12
#
#-------------------------------------------------

QT       += core gui
QT       += sql
QT       += network
QT += printsupport
CONFIG   += C++14
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DigitalLog
TEMPLATE = app
VERSION = 1.0.3.3
TEMPLATE = app
QMAKE_TARGET_COMPANY = Yankovich Andrei
QMAKE_TARGET_PRODUCT = DigitalLog
QMAKE_TARGET_DESCRIPTION = Digital Log
QMAKE_TARGET_COPYRIGHT = (c) Yankovich Andrei
RC_ICONS =logo.ico

SOURCES += main.cpp\
        mainwindow.cpp \
    EError.cpp \
    starastamode.cpp \
    groupmenager.cpp \
    setings.cpp \
    datewidget.cpp \
    readarchiv.cpp \
    about.cpp \
    help.cpp \
    sqldatabase.cpp \
    MounthDialog.cpp \
    Printer.cpp \
    SpaceEngineComponents/NetworkObjects/ELocalServers.cpp \
    SpaceEngineComponents/NetworkObjects/EServer.cpp \
    SpaceEngineComponents/NetworkObjects/ETcpSocket.cpp \
    SpaceEngineComponents/System/ELanguage.cpp \
    sourcetable.cpp \
    DataBasesMenager.cpp \
    PredmeMenager.cpp

HEADERS  += mainwindow.h \
    core.h \
    EError.h \
    starastamode.h \
    groupmenager.h \
    setings.h \
    datewidget.h \
    readarchiv.h \
    about.h \
    help.h \
    sqldatabase.h \
    MounthDialog.h \
    Printer.h \
    SpaceEngineComponents/NetworkObjects/ELocalServers.h \
    SpaceEngineComponents/NetworkObjects/EServer.h \
    SpaceEngineComponents/NetworkObjects/ETcpSocket.h \
    SpaceEngineComponents/System/ELanguage.h \
    sourcetable.h \
    languageindex.h \
    DataBasesMenager.h \
    PredmeMenager.h

FORMS +=

RESOURCES += \
    res.qrc \
    sqlres.qrc

DISTFILES += \
    sql/CreateDataBase.sql \
    sql/запросы.sql \
    sql/списки.sql