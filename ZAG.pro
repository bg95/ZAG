#-------------------------------------------------
#
# Project created by QtCreator 2013-10-14T21:49:27
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ZAG
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    BattleField/BFObject/BFObject.cpp \
    BattleField/BFObject/BFOCircle.cpp \
    BattleField/Vector2d.cpp \
    BattleField/BFManager.cpp \
    BattleField/BattleField.cpp \
    BattleField/IntersectionEvent.cpp \
    BattleField/BFObject/BFOColoredCircle.cpp \
    BattleField/BFController/BFController.cpp \
    BattleField/BFController/BFCHuman.cpp \
    BattleField/QuadTree.cpp \
    Network/server.cpp \
    Network/client.cpp

HEADERS  += MainWindow.h \
    BattleField/BFObject/BFObject.h \
    BattleField/BFObject/BFOCircle.h \
    BattleField/Vector2d.h \
    BattleField/BFManager.h \
    BattleField/BattleField.h \
    BattleField/IntersectionEvent.h \
    BattleField/BFObject/BFOColoredCircle.h \
    BattleField/BFController/BFController.h \
    BattleField/BFController/BFCHuman.h \
    BattleField/QuadTree.h \
    Network/server.h \
    Network/client.h\
    main.h

FORMS    += MainWindow.ui
