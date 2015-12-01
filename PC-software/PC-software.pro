#-------------------------------------------------
#
# Project created by QtCreator 2015-11-25T11:24:49
#
#-------------------------------------------------

QT       += core gui bluetooth

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Guntech-PC-Software
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    bluetoothserver.cpp

HEADERS  += mainwindow.h \
    bluetoothserver.h

FORMS    += mainwindow.ui
