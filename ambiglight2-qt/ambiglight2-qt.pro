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
    screenshot.cpp \
    ledconnector.cpp \
    xlibborderprovider.cpp \
    vlcdiskstreamborderprovider.cpp

HEADERS += \
    rgbconstructor.h \
    screenshot.h \
    dimensions.h \
    ledconnector.h \
    borderprovider.h \
    xlibborderprovider.h \
    vlcdiskstreamborderprovider.h
