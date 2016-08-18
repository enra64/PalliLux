#-------------------------------------------------
#
# Project created by QtCreator 2016-08-17T16:01:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ambilight-gui
TEMPLATE = app

# X11 stuff
CONFIG += link_pkgconfig
PKGCONFIG += x11

# magick++ stuff
QMAKE_CXXFLAGS += $(shell Magick++-config --cppflags --cxxflags)
LIBS += $(shell Magick++-config --ldflags --libs)

# fftw stuff
LIBS += -lfftw3

#pulseaudio
LIBS += -lpulse-simple
# pa_strerror is in here
LIBS += -lpulse

# amblight host library
HOST_PATH = /home/arne/Documents/Development/ShittyAmbilight/ambilight-host/
HOST_INC_PATH = $${HOST_PATH}/include
LIBS += $${HOST_PATH}/libhost.a
INCLUDEPATH += $${HOST_INC_PATH} \
    $${HOST_INC_PATH}/implementations/borderproviders \
    $${HOST_INC_PATH}/implementations/interfaces \
    $${HOST_INC_PATH}/implementations/rgblineprovider \
    $${HOST_INC_PATH}/implementations/screenshot



SOURCES += main.cpp\
    mainwindow.cpp \
    controldialog.cpp

HEADERS  += mainwindow.h \
    controldialog.h

FORMS    += mainwindow.ui \
    controldialog.ui
