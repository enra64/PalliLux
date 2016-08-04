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
    xlibborderprovider.cpp \
    vlcdiskstreamborderprovider.cpp \
    ambiconnector.cpp

HEADERS += \
    rgbconstructor.h \
    screenshot.h \
    dimensions.h \
    borderprovider.h \
    xlibborderprovider.h \
    vlcdiskstreamborderprovider.h \
    ledexceptions.h \
    ambiconnector.h
