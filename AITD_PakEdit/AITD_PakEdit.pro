#-------------------------------------------------
#
# Project created by QtCreator 2017-08-03T15:26:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AITD_PakEdit
TEMPLATE = app


SOURCES += main.cpp \
        ../src/pak.c \
        ../src/unpack.c \
        ../src/bmp.c \
        mainwindow.cpp

HEADERS  += mainwindow.h \
        ../src/pak.h \
        ../src/unpack.h \
        ../src/bmp.h \
        ../src/vars.h

FORMS    += mainwindow.ui


LIBS += -lSDL -lSDL_mixer -lGL -lGLU -lz
INCLUDEPATH += "/usr/include/SDL"
QMAKE_CXXFLAGS += -std=gnu++11
