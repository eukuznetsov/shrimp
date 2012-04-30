TARGET = shrimp

TEMPLATE = app
CONFIG += console
CONFIG -= qt app_bundle

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

unix:!macx:!symbian: LIBS += -lmysqlclient -lglog -lgflags -lboost_system -lboost_filesystem -lboost_regex
