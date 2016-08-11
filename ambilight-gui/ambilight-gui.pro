#-------------------------------------------------
#
# Project created by QtCreator 2016-08-08T11:27:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ambilight-gui
TEMPLATE = app

# X11 stuff
CONFIG += link_pkgconfig
PKGCONFIG += x11

#magick++ stuff
QMAKE_CXXFLAGS += $(shell Magick++-config --cppflags --cxxflags)
LIBS += $(shell Magick++-config --ldflags --libs)


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
