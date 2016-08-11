TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

# X11 stuff
CONFIG += link_pkgconfig
PKGCONFIG += x11

#magick++ stuff
QMAKE_CXXFLAGS += $(shell Magick++-config --cppflags --cxxflags)
LIBS += $(shell Magick++-config --ldflags --libs)

SOURCES += \
    main.cpp \
    xlibscreenshot.cpp \
    triplescreenborderprovider.cpp \
    vlcscreenshot.cpp \
    singlescreenborderprovider.cpp \
    ambirgblineprovider.cpp \
    arduinoconnector.cpp

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
    customexceptions.h
