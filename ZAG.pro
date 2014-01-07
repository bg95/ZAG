#-------------------------------------------------
#
# Project created by QtCreator 2013-10-14T21:49:27
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ZAG
TEMPLATE = app
CONFIG += c++11


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
    BattleField/BFRule/BFRule.cpp \
    BattleField/BFRule/BFRCollision.cpp \
    BattleField/BFFactory.cpp \
    BattleField/BFRule/BFRShoot.cpp \
    BattleField/BFController/BFCAIRandom.cpp

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
    main.h \
    BattleField/BFRule/BFRule.h \
    BattleField/BFRule/BFRCollision.h \
    BattleField/BFFactory.h \
    BattleField/BFRule/BFRShoot.h \
    BattleField/BFController/BFCAIRandom.h

FORMS    += MainWindow.ui
