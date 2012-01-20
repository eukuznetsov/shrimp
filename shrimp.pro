TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    inbase.cpp \
    error.cpp \
    inotify-cxx.cpp

HEADERS += \
    reader.h \
    inotify-cxx.h

