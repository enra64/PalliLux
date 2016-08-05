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
    rgbconstructor.cpp \
    main.cpp \
    ambiconnector.cpp \
    xlibscreenshot.cpp \
    triplescreenborderprovider.cpp \
    vlcscreenshot.cpp

HEADERS += \
    rgbconstructor.h \
    borderprovider.h \
    ledexceptions.h \
    ambiconnector.h \
    screenshot.h \
    xlibscreenshot.h \
    triplescreenborderprovider.h \
    vlcscreenshot.h \
    datafilter.h
