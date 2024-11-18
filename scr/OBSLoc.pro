#-------------------------------------------------
#
# Project created by QtCreator 2024-05-05T18:22:31
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = OBSLoc
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    inversion.cpp \
    forward.cpp \
    data_io.cpp \
    time_loc.cpp \
    geo2xyz.cpp \
    interpolation.cpp

HEADERS += \
    model.h \
    inversion.h \
    forward.h \
    data_io.h \
    time_loc.h \
    geo2xyz.h \
    interpolation.h
