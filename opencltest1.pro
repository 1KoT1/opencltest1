#-------------------------------------------------
#
# Project created by QtCreator 2014-07-08T21:17:37
#
#-------------------------------------------------
QMAKE_CXXFLAGS += -std=c++0x

QT       += core

QT       -= gui

TARGET = opencltest1
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

unix:!macx: LIBS += -L$$PWD/../../../../../opt/AMDAPP/lib/x86/ -lOpenCL

INCLUDEPATH += $$PWD/../../../../../opt/AMDAPP/include
DEPENDPATH += $$PWD/../../../../../opt/AMDAPP/include
