TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    inotify-cxx.cpp \
    inbasereader.cpp \
    mysqldatabase.cpp

HEADERS += \
    inotify-cxx.h \
    logreader.h \
    databaseexcp.h \
    mysqldatabase.h

INCLUDEPATH += /usr/include/mysql/

unix:!macx:!symbian: LIBS += -lmysqlclient
