#-------------------------------------------------
#
# Project created by QtCreator 2016-08-17T16:01:21
#
#-------------------------------------------------

QT       += core gui charts

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
    $${HOST_INC_PATH}/borderproviders \
    $${HOST_INC_PATH}/interfaces \
    $${HOST_INC_PATH}/rgblineprovider \
    $${HOST_INC_PATH}/datafilters \
    $${HOST_INC_PATH}/screenshot



SOURCES += main.cpp\
    mainwindow.cpp \
    controldialog.cpp \
    histogram.cpp

HEADERS  += mainwindow.h \
    controldialog.h \
    histogram.h

FORMS    += mainwindow.ui \
    controldialog.ui
