#-------------------------------------------------
#
# Project created by QtCreator 2013-10-14T21:49:27
#
#-------------------------------------------------

QT       += core gui opengl
QT += network widgets

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
    Network/Client.cpp \
    Network/Server.cpp\
    BattleField/BFFactory.cpp \
    BattleField/BFRule/BFRShoot.cpp \
    BattleField/BFController/BFCAIRandom.cpp \
    BattleField/Geometry.cpp \
    BattleField/BFController/BFCRandomShootDodge.cpp \
    BattleField/ControlEvent.cpp \
    BattleField/BFRule/BFRSurvival.cpp \
    BattleField/BFController/BFCHumanAndRSD.cpp \
    global.cpp \
    BattleField/Scene.cpp \
    Network/Connection.cpp\
    BattleField/BFController/BFCHumanAndRSD.cpp

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
    Network/Client.h \
    Network/Server.h \
    BattleField/BFFactory.h \
    BattleField/BFRule/BFRShoot.h \
    BattleField/BFController/BFCAIRandom.h \
    BattleField/Geometry.h \
    BattleField/BFController/BFCRandomShootDodge.h \
    BattleField/ControlEvent.h \
    BattleField/BFRule/BFRSurvival.h \
    global.h \
    BattleField/BFController/BFCHumanAndRSD.h \
    BattleField/Scene.h \
    Network/Connection.h\
    BattleField/BFController/BFCHumanAndRSD.h

FORMS    += MainWindow.ui

INCLUDEPATH += -I /usr/include/
