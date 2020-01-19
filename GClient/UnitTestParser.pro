#-------------------------------------------------
#
# Project created by QtCreator 2011-06-01T19:32:02
#
#-------------------------------------------------

QT       += network testlib

TARGET = tst_unittestparsertest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_unittestparsertest.cpp \
    serverManager/ServerManager.cpp \
    serverManager/Parser.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    serverManager/ServerManager.h \
    serverManager/Parser.h \
    message/MessageTimeToStartGame.h \
    message/MessageStepPlayer.h \
    message/MessageStateMap.h \
    message/MessageStartMapGame.h \
    message/MessageItf.h \
    message/MessageFinishGame.h \
    message/MessageError.h \
    message/MessageConnectToServer.h \
    message/MessageAddView.h
