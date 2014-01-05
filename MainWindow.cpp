#include "BattleField/BFObject/BFOCircle.h"
#include "BattleField/BFObject/BFOColoredCircle.h"
#include "BattleField/BFController/BFCHuman.h"
#include "BattleField/BFController/AI/BFCAIRandom.h"

#include "BattleField/BFRule/BFRCollision.h"
#include "BattleField/BFRule/BFRShoot.h"

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <sstream>

#include "main.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)//,
    //ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
    setGeometry(20, 20, 700, 700);

    bf = new BattleField(this);
    bf->setGeometry(0, 0, width(), height());

    //rule = new BFRCollision(bf->getManager());
    rule = new BFRShoot(bf->getManager());
    bf->getManager()->setRule(rule);

    BFOColoredCircle *circle;
/*
    circle = new BFOColoredCircle(bf->getManager());
    circle->p = Vector2d(0.89, 0.05);
    circle->r = 0.1;
    circle->v = Vector2d(-0.4, 0);
    circle->m = 1;
    circle->maxa = 5;
    bf->getManager()->insertObject(circle);
    BFCHuman *hum = new BFCHuman(circle);
    bf->getManager()->registerController(hum);*/
/*
    circle = new BFOColoredCircle(bf->getManager());
    circle->p = Vector2d(0, -0.05);
    circle->r = 0.1;
    circle->v = Vector2d(0.4, 0);
    circle->m = 1;
    bf->getManager()->insertObject(circle);

    circle = new BFOColoredCircle(bf->getManager());
    circle->p = Vector2d(-0.89, 0.03);
    circle->r = 0.14;
    circle->v = Vector2d(0.4, 0.1);
    circle->m = 2;
    bf->getManager()->insertObject(circle);
*//*
    circle = new BFOColoredCircle(bf->getManager());
    circle->p = Vector2d(-0.89, 0.03);
    circle->r = 0.05;
    circle->v = Vector2d(0.8, 0.5);
    circle->m = 5;
    bf->getManager()->insertObject(circle);
*/

    for (int i = 0; i < 5; i++)
    {
        circle = new BFOColoredCircle;//(bf->getManager());
        circle->p = Vector2d(i / 8.0 - 0.9, 0.9);
        circle->r = 0.05;
        circle->v = Vector2d(0.8, 0.5);
        circle->m = 0.25;
        circle->setProperty("shoot", "");
        circle->setProperty("health", 1.0);
        bf->getManager()->insertObject(circle);
        circles[i] = circle;
    }

    BFOColoredCircle *bullet = new BFOColoredCircle();
    bullet->setColor(1.0, 0, 0, 1.0);
    bullet->r = 0.005;
    bullet->v = Vector2d(0, 6);
    bullet->m = 0.05;
    bullet->setProperty("isBullet", "Yes");
    bullet->setProperty("damage", 0.2);

    circle = new BFOColoredCircle;//(bf->getManager());
    bullet->setProperty("shooter", (unsigned long long)circle);
    circle->p = Vector2d(0, 0.9);
    circle->r = 0.05;
    circle->v = Vector2d(0, 0.5);
    circle->m = 0.25;
    circle->maxa = 5;
    circle->setColor(0.0, 0.5, 1.0, 1.0);
    circle->setProperty("shoot", "");
    circle->setProperty("bullet prototype", (unsigned long long)bullet);
    circle->setProperty("cooldown", 0.05);
    circle->setProperty("cooldowncount", 0.0);
    circle->setProperty("health", 1.0);
    bf->getManager()->insertObject(circle);

    BFCHuman *hum = new BFCHuman(circle);
    //BFCAIRandom *air = new BFCAIRandom(bf->getManager(), circle);
    bf->getManager()->registerController(hum);

    circle = new BFOColoredCircle;//(bf->getManager());
    bullet->setProperty("shooter", (unsigned long long)circle);
    circle->p = Vector2d(0, 0.8);
    circle->r = 0.05;
    circle->v = Vector2d(0, 0.5);
    circle->m = 0.25;
    circle->maxa = 5;
    circle->setColor(0.0, 0.5, 1.0, 1.0);
    circle->setProperty("shoot", "");
    circle->setProperty("bullet prototype", (unsigned long long)bullet);
    circle->setProperty("cooldown", 0.05);
    circle->setProperty("cooldowncount", 0.0);
    circle->setProperty("health", 1.0);
    bf->getManager()->insertObject(circle);

    hum = new BFCHuman(circle);
    bf->getManager()->registerController(hum);

    for (int i = 0; i < 5; i++)
    {
        circle = new BFOColoredCircle;//(bf->getManager());
        circle->p = Vector2d(-(i / 8.0 - 0.9), 0.9);
        circle->r = 0.05;
        circle->v = Vector2d(-0.8, 0.5);
        circle->m = 0.25;
        circle->setProperty("shoot", "");
        circle->setProperty("health", 1.0);
        bf->getManager()->insertObject(circle);
        circles[i] = circle;
    }

    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 50; j++)
        {
            circle = new BFOColoredCircle;//(bf->getManager());
            circle->p = Vector2d(i / 80.0 - 0.5, -0.9 + j / 80.0);
            circle->r = 0.005;
            circle->v = Vector2d(i / 350.0 - 0.5, 0.5 + i / 180.0 - 0.5);
            circle->m = 0.0025;
            circle->setProperty("shoot", "");
            circle->setProperty("health", 0.4);
            bf->getManager()->insertObject(circle);
        }

    //bf->refresh();
    bf->start();
}

MainWindow::~MainWindow()
{
    //delete ui;
    delete bf;
    delete rule;
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    bf->setGeometry(0, 0, width(), height());
}
