#-------------------------------------------------
#
# Project created by QtCreator 2011-05-30T18:44:01
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_unittestfinaltest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_unittestfinaltest.cpp \
    Planet.cpp \
    Fleet.cpp \
    Space.cpp \
    Game.cpp \
    Player.cpp
#DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    Planet.h \
    Fleet.h \
    Space.h \
    message/MessageStateMap.h \
    message/MessageStartMapGame.h \
    Game.h \
    message/MessageAddView.h \
    SceneUpdates.h \
    message/MessageFinishGame.h \
    Player.h
