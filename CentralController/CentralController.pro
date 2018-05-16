#-------------------------------------------------
#
# Project created by QtCreator 2018-05-15T17:07:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CentralController
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        view/mainwindow.cpp \
    3rdparty/util.cpp \
    model/MCan.cpp \
    model/MUart.cpp \
    interface/IBus.cpp \
    protocol/PModbus.cpp \
    protocol/PCan.cpp \
    interface/CanBus.cpp \
    interface/ModBus.cpp

HEADERS += \
        view/mainwindow.h \
    3rdparty/util.h \
    3rdparty/qtbase.h \
    model/MCan.h \
    model/MUart.h \
    interface/IBus.h \
    protocol/PModbus.h \
    protocol/PCan.h \
    interface/CanBus.h \
    interface/ModBus.h

FORMS += \
        view/mainwindow.ui
