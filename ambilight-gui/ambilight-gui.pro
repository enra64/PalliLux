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
AMBILIGHT_LIB_FOLDER = ../ambilight-host/

SOURCES += main.cpp\
    mainwindow.cpp \
    controldialog.cpp \
    singlescreenconfigpage.cpp \
    triplescreenconfigpage.cpp \
    letterboxingautoconfigdialog.cpp \
    fpsmeter.cpp \
    backlightconfigpage.cpp \
    backlightdialog.cpp \
    pixellinewidget.cpp \
    histogramwidget.cpp

HEADERS  += mainwindow.h \
    controldialog.h \
    singlescreenconfigpage.h \
    iconfigpage.h \
    triplescreenconfigpage.h \
    letterboxingautoconfigdialog.h \
    fpsmeter.h \
    backlightconfigpage.h \
    backlightdialog.h \
    pixellinewidget.h \
    histogramwidget.h \
    screenshotfactory.h \
    cimg/CImg.h \
    include_img_lib.h

FORMS    += mainwindow.ui \
    controldialog.ui \
    singlescreenconfigpage.ui \
    triplescreenconfigpage.ui \
    letterboxingautoconfigdialog.ui \
    backlightconfigpage.ui \
    backlightdialog.ui

# check whether we can use qt charts
qtHaveModule(charts){
    QT += charts
    DEFINES += QT_CHARTS_FOUND
    message("qt charts component found")
}

win32 {
    # windows screenshot stuff
    LIBS += -luser32 -lgdi32

    # ignore unknown pragmas in stuff not written by me
    #QMAKE_CXXFLAGS += -Wnounknown-pragmas

    # static ambilight lib
    LIBS += $${AMBILIGHT_LIB_FOLDER}/build64/Debug/host.lib
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

    # static ambilight lib
    LIBS += "/home/arne/Documents/Development/ShittyAmbilight/build-ambilight-host-Desktop-Default/libhost.a"
}

INCLUDEPATH += $${AMBILIGHT_LIB_FOLDER}/include \
    $${AMBILIGHT_LIB_FOLDER}/include/borderproviders \
    $${AMBILIGHT_LIB_FOLDER}/include/interfaces \
    $${AMBILIGHT_LIB_FOLDER}/include/colordataproviders \
    $${AMBILIGHT_LIB_FOLDER}/include/datafilters \
    $${AMBILIGHT_LIB_FOLDER}/include/serial \
    $${AMBILIGHT_LIB_FOLDER}/include/screenshotproviders
