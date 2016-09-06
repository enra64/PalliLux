#-------------------------------------------------
#
# Project created by QtCreator 2016-08-17T16:01:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ambilight-gui
TEMPLATE = app

# amblight host library folder location
AMBILIGHT_LIB_PATH = ../ambilight-host/

SOURCES += main.cpp\
    mainwindow.cpp \
    controldialog.cpp \
    singlescreenconfigpage.cpp \
    triplescreenconfigpage.cpp \
    letterboxingautoconfigdialog.cpp \
    fpsmeter.cpp

HEADERS  += mainwindow.h \
    controldialog.h \
    singlescreenconfigpage.h \
    iconfigpage.h \
    triplescreenconfigpage.h \
    letterboxingautoconfigdialog.h \
    fpsmeter.h

FORMS    += mainwindow.ui \
    controldialog.ui \
    singlescreenconfigpage.ui \
    triplescreenconfigpage.ui \
    letterboxingautoconfigdialog.ui \
    fpsmeter.ui

# check whether we can use qt charts
qtHaveModule(charts){
    QT += charts
    DEFINES += QT_CHARTS_FOUND
    message("qt charts component found")
}

win32 {
    # windows screenshot stuff
    LIBS += -luser32 -lgdi32

    # windows imagemagick - sadly hardcoded, as i could not find a way to do this properly
    LIBS += "C:\Program Files\ImageMagick-7.0.2-Q16\lib\CORE_RL_Magick++_.lib"
    LIBS += "C:\Program Files\ImageMagick-7.0.2-Q16\lib\CORE_RL_MagickCore_.lib"
    INCLUDEPATH += "C:\Program Files\ImageMagick-7.0.2-Q16\include"

    # static ambilight lib
    LIBS += $${AMBILIGHT_LIB_PATH}/build/Debug/host.lib
}

unix {
    # X11 stuff
    CONFIG += link_pkgconfig
    PKGCONFIG += x11

    # fftw stuff
    LIBS += -lfftw3

    # pulseaudio
    LIBS += -lpulse-simple
    # pa_strerror is in here
    LIBS += -lpulse

    # magick++
    QMAKE_CXXFLAGS += $(shell Magick++-config --cppflags --cxxflags)
    LIBS += $(shell Magick++-config --ldflags --libs)

    # static ambilight lib
    LIBS += $${AMBILIGHT_LIB_PATH}/libhost.a
}

INCLUDEPATH += $${AMBILIGHT_LIB_PATH}/include \
    $${AMBILIGHT_LIB_PATH}/include/borderproviders \
    $${AMBILIGHT_LIB_PATH}/include/interfaces \
    $${AMBILIGHT_LIB_PATH}/include/rgblineprovider \
    $${AMBILIGHT_LIB_PATH}/include/datafilters \
    $${AMBILIGHT_LIB_PATH}/include/serial \
    $${AMBILIGHT_LIB_PATH}/include/screenshot
