TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    inotify-cxx.cpp \
    inbasereader.cpp \
    inbasereadererror.cpp \
    mysqldatabase.cpp

HEADERS += \
    inotify-cxx.h \
    logreader.h \
    databaseexcp.h \
    basedatabase.h \
    mysqldatabase.h

