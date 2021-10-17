#-------------------------------------------------
#
# Project created by QtCreator 2017-08-03T15:26:44
#
#-------------------------------------------------

QT       += core gui xml
CONFIG += debug
#CONFIG += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AITD_PakEdit
TEMPLATE = app


SOURCES += main.cpp \
        ../src/pak.c \
        ../src/unpack.c \
        ../src/bmp.c \
        mainwindow.cpp \
    alonefile.cpp \
    pakfile.cpp \
    jsoncpp.cpp \
    db.cpp \
    alonefloor.cpp \
    ../src/room.c \
    ../src/vars.c \
    ../src/floor.c \
    ../src/freezetime.c \
    ../src/hqr.c \
    ../src/fileAccess.c \
    ../src/cosTable.c \
    alonebody.cpp \
	settings.cpp

HEADERS  += mainwindow.h \
        ../src/pak.h \
        ../src/unpack.h \
        ../src/bmp.h \
        ../src/vars.h \
    alonefile.h \
    pakfile.h \
    json/json-forwards.h \
    json/json.h \
    ../src/types.h \
    db.h \
    alonefloor.h \
    ../src/room.h \
    ../src/floor.h \
    ../src/freezetime.h \
    ../src/hqr.h \
    ../src/fileAccess.h \
    alonebody.h \
	settings.h

FORMS    += mainwindow.ui

win32:INCLUDEPATH = C:\libs\
win32:LIBS =        C:\libs\


LIBS += -lz
QMAKE_CXXFLAGS += -std=gnu++11

RESOURCES += \
    data.qrc
