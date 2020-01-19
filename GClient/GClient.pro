#-------------------------------------------------
#
# Project created by QtCreator 2011-05-30T14:00:46
#
#-------------------------------------------------

QT       += core gui network

TARGET = GClient
TEMPLATE = app


SOURCES += main.cpp\
        galconclient.cpp \
    serverManager/ServerManager.cpp \
    serverManager/Parser.cpp

HEADERS  += galconclient.h \
    serverManager/ServerManager.h \
    serverManager/Parser.h \
    message/MessageItf.h \
    message/MessageAddView.h \
    message/MessageConnectToServer.h \
    message/MessageError.h \
    message/MessageFinishGame.h \
    message/MessageStartMapGame.h \
    message/MessageStateMap.h \
    message/MessageTimeToStartGame.h \
    message/MessageStepPlayer.h \
    message/MessageStateMap.h \
    message/MessageStartMapGame.h \
    message/MessageItf.h \
    message/MessageFinishGame.h \
    message/MessageError.h \
    message/MessageConnectToServer.h \
    message/MessageAddView.h \
    message/MessageInformation.h

FORMS    += galconclient.ui
