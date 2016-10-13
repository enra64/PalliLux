#-------------------------------------------------
#
# Project created by QtCreator 2016-08-17T16:01:21
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pallilux-gui
TEMPLATE = app

# amblight host library folder location
PALLILUX_LIB_FOLDER = ../pallilux/

SOURCES += main.cpp\
    mainwindow.cpp \
    singlescreenconfigpage.cpp \
    triplescreenconfigpage.cpp \
    letterboxingautoconfigdialog.cpp \
    fpsmeter.cpp \
    backlightconfigpage.cpp \
    pixellinewidget.cpp \
    histogramwidget.cpp \
    controlwidget.cpp \
    ambicontrolwidget.cpp \
    backlightcontrolwidget.cpp \
    ledconfigdialog.cpp \
    serialconfigdialog.cpp

HEADERS  += mainwindow.h \
    singlescreenconfigpage.h \
    iconfigpage.h \
    triplescreenconfigpage.h \
    letterboxingautoconfigdialog.h \
    fpsmeter.h \
    backlightconfigpage.h \
    pixellinewidget.h \
    histogramwidget.h \
    screenshotfactory.h \
    cimg/CImg.h \
    include_img_lib.h \
    controlwidget.h \
    ambicontrolwidget.h \
    backlightcontrolwidget.h \
    ledconfigdialog.h \
    serialconfigdialog.h

FORMS    += mainwindow.ui \
    singlescreenconfigpage.ui \
    triplescreenconfigpage.ui \
    letterboxingautoconfigdialog.ui \
    backlightconfigpage.ui \
    controlwidget.ui \
    ledconfigdialog.ui \
    serialconfigdialog.ui

# check whether we can use qt charts
qtHaveModule(charts){
    if(!defined(DISABLE_QT_CHARTS)){
        QT += charts
        DEFINES += QT_CHARTS_FOUND
        message("qt charts component found")
    } else {
        message("qt charts component found, but disabled due to DISABLE_QT_CHARTS")
    }
}

win32 {
    # windows screenshot stuff
    LIBS += -luser32 -lgdi32

    # ignore unknown pragmas in stuff not written by me
    #QMAKE_CXXFLAGS += -Wnounknown-pragmas

    # static pallilux lib
    LIBS += $${PALLILUX_LIB_FOLDER}/build64/Debug/host.lib
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

    # static pallilux lib
    LIBS += "/home/arne/Documents/Development/PalliLux/build-pallilux-Desktop-Release/libpallilux.a"

    # dynamic pallilux lib
    #LIBS += -L/home/arne/Documents/Development/PalliLux/build-pallilux-host-Desktop-Debug -lhost
}

INCLUDEPATH += $${PALLILUX_LIB_FOLDER}/cimg \
    $${PALLILUX_LIB_FOLDER}/include \
    $${PALLILUX_LIB_FOLDER}/include/borderproviders \
    $${PALLILUX_LIB_FOLDER}/include/interfaces \
    $${PALLILUX_LIB_FOLDER}/include/colordataproviders \
    $${PALLILUX_LIB_FOLDER}/include/datafilters \
    $${PALLILUX_LIB_FOLDER}/include/serial \
    $${PALLILUX_LIB_FOLDER}/include/screenshotproviders
