#include "BattleField/BFObject/BFOCircle.h"
#include "BattleField/BFObject/BFOColoredCircle.h"
#include "BattleField/BFController/BFCHuman.h"
#include "BattleField/BFController/BFCAIRandom.h"

#include "BattleField/BFRule/BFRCollision.h"
#include "BattleField/BFRule/BFRShoot.h"

#include "BattleField/BFFactory.h"

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <sstream>
#include <QBuffer>

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
    BFCAIRandom *controller;

    BFOColoredCircle *bullet = new BFOColoredCircle();
    bullet->setColor(1.0, 0, 0, 1.0);
    bullet->r = 0.002;
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
    circle->setProperty("cooldown", 0.0);
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

    QBuffer *buf = new QBuffer;
    BFFactory *fac = bf->getManager()->getFactory();
    circle = new BFOColoredCircle;//(bf->getManager());
    circle->p = Vector2d(-0.7, 0.9);
    circle->r = 0.05;
    circle->v = Vector2d(0.8, 0.5);
    circle->m = 0.25;
    circle->maxa = 5;
    circle->setProperty("shoot", "");
    circle->setProperty("health", 1.0);
    buf->open(QBuffer::WriteOnly);
    fac->encodeObject(circle, buf);
    buf->close();
    qDebug("prototype id = %ld", circle->getID());
    delete circle;

    for (int i = 0; i < 5; i++)
    {
        buf->open(QBuffer::ReadOnly);
        buf->seek(0);
        circle = (BFOColoredCircle *)fac->decodeNewObject(buf);
        circle->p = Vector2d(i / 8.0 - 0.8, 0.9);/*
        qDebug("%lX", (unsigned long)circle);
        qDebug("%lf,%lf", i / 8.0 - 0.8, 0.9);
        //circle->p.x = i / 8.0 - 0.8;
        qDebug("%lf,%lf", circle->p.x, circle->p.y);
        qDebug("health: %lf", circle->getProperty("health").toDouble());*/
        bf->getManager()->insertObject(circle);
        controller = new BFCAIRandom(circle);
        bf->getManager()->registerController(controller);
        circles[i] = circle;
        buf->close();
    }
    delete buf;
    for (int i = 0; i < 5; i++)
    {
        circle = new BFOColoredCircle;//(bf->getManager());
        circle->p = Vector2d(-(i / 8.0 - 0.9), 0.9);
        circle->r = 0.05;
        circle->v = Vector2d(-0.8, 0.5);
        circle->m = 0.25;
        circle->maxa = 5;
        circle->setProperty("shoot", "");
        circle->setProperty("health", 1.0);
        bf->getManager()->insertObject(circle);
        //controller = new BFCAIRandom(circle);
        //bf->getManager()->registerController(controller);
        //circles[i] = circle;
    }
/*
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 5; j++)
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
*/
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
