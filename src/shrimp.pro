TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += main.cpp \
    inotify-cxx.cpp \
    mysqldatabase.cpp \
    logreaderadd.cpp \
    parserinotify.cpp \
    baseparser.cpp

HEADERS += \
    inotify-cxx.h \
    logreader.h \
    databaseexcp.h \
    mysqldatabase.h

INCLUDEPATH += /usr/include/mysql/

unix:!macx:!symbian: LIBS += -lmysqlclient -lboost_system -lboost_filesystem
