#-------------------------------------------------
#
# Project created by QtCreator 2016-08-17T16:01:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gui
TEMPLATE = app

# add includepath to host headers
INCLUDEPATH += $$PWD/../host/include/
INCLUDEPATH += $$PWD/../host/include/implementations
INCLUDEPATH += $$PWD/../host/include/implementations/interfaces

# magick++ stuff
QMAKE_CXXFLAGS += $(shell Magick++-config --cppflags --cxxflags)
LIBS += $(shell Magick++-config --ldflags --libs)

SOURCES += main.cpp\
        mainwindow.cpp \
    controldialog.cpp

HEADERS  += mainwindow.h \
    controldialog.h

FORMS    += mainwindow.ui \
    controldialog.ui
