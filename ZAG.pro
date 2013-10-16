#-------------------------------------------------
#
# Project created by QtCreator 2013-10-14T21:49:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ZAG
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    BattleField/BFObject.cpp \
    BattleField/BFOCircle.cpp \
    BattleField/Vector2d.cpp \
    BattleField/BFManager.cpp

HEADERS  += MainWindow.h \
    BattleField/BFObject.h \
    BattleField/BFOCircle.h \
    BattleField/Vector2d.h \
    BattleField/BFManager.h

FORMS    += MainWindow.ui
