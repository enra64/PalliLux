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
    # spectrometer stuff
    SOURCES +=  spectrometerconfigpage.cpp \
                spectrometercontrolwidget.cpp

    HEADERS +=  spectrometerconfigpage.h \
                spectrometercontrolwidget.h

    FORMS +=    spectrometerconfigpage.ui


    # X11 stuff
    CONFIG += link_pkgconfig
    PKGCONFIG += x11

    # remove possible other optimization flags
    QMAKE_CXXFLAGS_RELEASE -= -O
    QMAKE_CXXFLAGS_RELEASE -= -O1
    QMAKE_CXXFLAGS_RELEASE -= -O2

    # add the desired -O3 if not present
    QMAKE_CXXFLAGS_RELEASE *= -O3

    # static pallilux lib, using debug or release build
    CONFIG( debug, debug|release){
        message("debug")
        LIBS += "/home/arne/Documents/Development/PalliLux/build-pallilux-Desktop-Debug/libpallilux.a"
    }
    CONFIG(release, debug|release) {
        message("release")
        QMAKE_LFLAGS -= as-needed
        LIBS += "/home/arne/Documents/Development/PalliLux/build-pallilux-Desktop-Release/libpallilux.a"
    }

    # fftw, pulseaudio stuff
    LIBS += -L/usr/lib64 -lfftw3 -lfftw3f -lm -lpulse-simple -lpulse

    TEST = $${LIBS}
    message(libs content is $$TEST)
}

INCLUDEPATH += $${PALLILUX_LIB_FOLDER}/cimg \
    $${PALLILUX_LIB_FOLDER}/include \
    $${PALLILUX_LIB_FOLDER}/include/borderproviders \
    $${PALLILUX_LIB_FOLDER}/include/interfaces \
    $${PALLILUX_LIB_FOLDER}/include/colordataproviders \
    $${PALLILUX_LIB_FOLDER}/include/colordataproviders/spectrometer \
    $${PALLILUX_LIB_FOLDER}/include/colordataproviders/spectrometer/colormappers \
    $${PALLILUX_LIB_FOLDER}/include/datafilters \
    $${PALLILUX_LIB_FOLDER}/include/serial \
    $${PALLILUX_LIB_FOLDER}/include/screenshotproviders

RESOURCES += \
    images.qrc
