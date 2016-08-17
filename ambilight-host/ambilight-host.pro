TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

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

SOURCES += \
    main.cpp \
    xlibscreenshot.cpp \
    triplescreenborderprovider.cpp \
    vlcscreenshot.cpp \
    singlescreenborderprovider.cpp \
    ambirgblineprovider.cpp \
    arduinoconnector.cpp \
    spectrometerrgblineprovider.cpp

HEADERS += \
    borderprovider.h \
    screenshot.h \
    xlibscreenshot.h \
    triplescreenborderprovider.h \
    vlcscreenshot.h \
    datafilter.h \
    singlescreenborderprovider.h \
    rgblineprovider.h \
    ambirgblineprovider.h \
    arduinoconnector.h \
    customexceptions.h \
    spectrometerrgblineprovider.h \
    lest.hpp
