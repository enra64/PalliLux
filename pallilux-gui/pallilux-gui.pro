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
    rotatingbacklightcontrolwidget.cpp \
    ledconfigdialog.cpp \
    serialconfigdialog.cpp \
    rotatingbacklightconfigpage.cpp

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
    rotatingbacklightcontrolwidget.h \
    ledconfigdialog.h \
    serialconfigdialog.h \
    rotatingbacklightconfigpage.h

FORMS    += mainwindow.ui \
    singlescreenconfigpage.ui \
    triplescreenconfigpage.ui \
    letterboxingautoconfigdialog.ui \
    backlightconfigpage.ui \
    controlwidget.ui \
    ledconfigdialog.ui \
    serialconfigdialog.ui \
    rotatingbacklightconfigpage.ui

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

    # dx stuff
    LIBS += "C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x64\d3d11.lib"
    LIBS += "C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x64\dxgi.lib"

    # static pallilux lib, using debug or release build
    CONFIG( debug, debug|release){
    message("debug")
        LIBS += "C:\Users\arne\Downloads\PalliLux\build-pallilux-Desktop_Qt_5_7_0_MSVC2015_64bit-Debug\pallilux.lib"
    }
    CONFIG(release, debug|release) {
        message("release")
        LIBS += "C:\Users\arne\Downloads\PalliLux\build-pallilux-Desktop_Qt_5_7_0_MSVC2015_64bit-Release\pallilux.lib"
    }
}

unix {
if(0){
    # fftw, pulseaudio stuff
    LIBS += -L/usr/lib64 -lfftw3 -lpulse-simple -lpulse

    # spectrometer stuff
    SOURCES +=  spectrometerconfigpage.cpp \
                spectrometercontrolwidget.cpp

    HEADERS +=  spectrometerconfigpage.h \
                spectrometercontrolwidget.h

    FORMS +=    spectrometerconfigpage.ui
}


    # X11 stuff
    CONFIG += link_pkgconfig
    PKGCONFIG += x11

    # static pallilux lib, using debug or release build
    CONFIG( debug, debug|release){
        message("debug")
        LIBS += "/home/arne/Documents/Development/PalliLux/build-pallilux-Desktop-Debug/libpallilux.a"
    }
    CONFIG(release, debug|release) {
        message("release")
        LIBS += "/home/arne/Documents/Development/PalliLux/build-pallilux-Desktop-Release/libpallilux.a"
    }
}

INCLUDEPATH += $${PALLILUX_LIB_FOLDER}/cimg \
    $${PALLILUX_LIB_FOLDER}/include \
    $${PALLILUX_LIB_FOLDER}/include/borderproviders \
    $${PALLILUX_LIB_FOLDER}/include/interfaces \
    $${PALLILUX_LIB_FOLDER}/include/colordataproviders \
    $${PALLILUX_LIB_FOLDER}/include/datafilters \
    $${PALLILUX_LIB_FOLDER}/include/serial \
    $${PALLILUX_LIB_FOLDER}/include/screenshotproviders

RESOURCES += \
    images.qrc
