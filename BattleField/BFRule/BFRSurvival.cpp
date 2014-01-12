#include "../BFObject/BFOColoredCircle.h"
#include "../BFController/BFCRandomShootDodge.h"
#include "../BFController/BFCHumanAndRSD.h"
#include "global.h"

#include <QBuffer>

#include "BFRSurvival.h"

BFRSurvival::BFRSurvival(BFManager *_manager):
    BFRShoot(_manager)
{
    counter = 0;
    difficulty = 0;
    strength = 0;
}

BFRSurvival::~BFRSurvival()
{
}

int BFRSurvival::getNumberFractions() const
{
    return 1;
}

void BFRSurvival::processInput()
{
    manager->destructObjects();
    manager->destructControllers();
    if (difficulty == 0)
    {
        generateInitialObjects();
        difficulty = 1;
    }

    BFRShoot::processInput();

    counter++;
    if (counter % 100 == 0)
    {
        generateObjectByStrength(difficulty);
        difficulty++;
    }
}

void BFRSurvival::generateInitialObjects()
{
    BFOColoredCircle *circle;
    std::vector<BFObjectID> objid;

    BFOColoredCircle *bullet = (BFOColoredCircle *)manager->getFactory()->newObject(typehash(BFOColoredCircle));
    bullet->setColor(1.0, 0, 0, 1.0);
    bullet->r = 0.01;
    bullet->v = Vector2d(0, 6);
    bullet->m = 0.01;
    bullet->setProperty("isBullet", "Yes");
    bullet->setProperty("damage", 0.2);
    QBuffer bulletbuf;
    bulletbuf.open(QIODevice::ReadWrite);
    manager->getFactory()->encodeObject(bullet, &bulletbuf);
    bulletbuf.seek(0);
    manager->getFactory()->deleteObject(bullet);
    //shooter property is set in BFRShoot

    //circle = new BFOColoredCircle;//(manager);
    circle = (BFOColoredCircle *)manager->getFactory()->newObject(typehash(BFOColoredCircle));
    //bullet->setProperty("shooter", (unsigned long long)circle);
    circle->p = Vector2d(0, 0.9);
    circle->r = 0.1;
    circle->v = Vector2d(0, 0.5);
    circle->m = 1;
    circle->maxa = 5;
    circle->setColor(0.0, 0.5, 1.0, 1.0);
    circle->setProperty("shoot", "");
    circle->setProperty("bullet prototype", bulletbuf.data());
    circle->setProperty("cooldown", 0.05);
    circle->setProperty("cooldowncount", 0.0);
    circle->setProperty("health", 1.0);
    circle->setProperty("fraction", 0);
    manager->insertObject(circle);
    objid.push_back(circle->getID());

    manager->getBattleField()->setFocusObject(circle->getID());

    QBuffer *buf = new QBuffer;
    BFFactory *fac = manager->getFactory();
    circle = (BFOColoredCircle *)manager->getFactory()->newObject(typehash(BFOColoredCircle));//(manager);
    circle->p = Vector2d(-0.7, 0.9);
    circle->r = 0.05;
    circle->v = Vector2d(0.8, 0.5);
    circle->m = 0.25;
    circle->maxa = 5;
    circle->setProperty("shoot", "");
    circle->setProperty("health", 1.0);
    //(*circle)["fraction"] = 2;
    circle->setProperty("fraction", 0);
    buf->open(QBuffer::WriteOnly);
    fac->encodeObject(circle, buf);
    buf->close();
    qDebug("prototype id = %ld", circle->getID());
    //delete circle;
    manager->getFactory()->deleteObject(circle);

    buf->open(QBuffer::ReadOnly);
    for (int i = 0; i < 5; i++)
    {
        buf->seek(0);
        circle = (BFOColoredCircle *)fac->decodeNewObject(buf);
        circle->p = Vector2d(i / 8.0 - 0.9, 0.9);
        manager->insertObject(circle);
        objid.push_back(circle->getID());
        //controller = new BFCAIRandom(manager, circle->getID());
        //manager->registerController(controller);
        //circles[i] = circle;
    }
    for (int i = 0; i < 5; i++)
    {
        buf->seek(0);
        circle = (BFOColoredCircle *)fac->decodeNewObject(buf);
        circle->p = Vector2d(-(i / 8.0 - 0.9), 0.9);
        manager->insertObject(circle);
        objid.push_back(circle->getID());
        //controller = new BFCAIRandom(manager, circle->getID());
        //manager->registerController(controller);
        //circles[i] = circle;
    }
    buf->close();
    delete buf;
    BFController *ctrl = new BFCHumanAndRSD(manager, objid);
    manager->registerController(ctrl);
/*
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 5; j++)
        {
            circle = new BFOColoredCircle;//(manager);
            circle->p = Vector2d(i / 80.0 - 0.5, -0.9 + j / 80.0);
            circle->r = 0.005;
            circle->v = Vector2d(i / 350.0 - 0.5, 0.5 + i / 180.0 - 0.5);
            circle->m = 0.0025;
            circle->setProperty("shoot", "");
            circle->setProperty("health", 0.4);
            manager->insertObject(circle);
        }
    bf -> update();
*/
}

void BFRSurvival::generateObjectByStrength(int str)
{
    str++;

    double r = sqrt((double)str) / 30.0;
    double m = 10.0 * r * r;
    double maxa = 0.05 / r;
    double health = m * 2;

    double bulletr = r / 10.0;
    double bulletm = 1.0 * bulletr * bulletr;
    double bulletv = 0.05 / bulletr;
    double damage = bulletm * 20.0;
    double cooldown = 0.5;

    BFOColoredCircle *circle;
    BFController *controller;
    circle = (BFOColoredCircle *)manager->getFactory()->newObject(typehash(BFOColoredCircle));
    circle->r = r;
    int i;
    for (i = 0; i < 10 && manager->intersectingWithAnyObject(circle); i++)
        circle->p = Vector2d(2.0 * (double)rand() / RAND_MAX - 1.0, 2.0 * (double)rand() / RAND_MAX - 1.0);
    if (i >= 10)
    {
        manager->getFactory()->deleteObject(circle);
        return;
    }
    circle->v = Vector2d(2.0 * (double)rand() / RAND_MAX - 1.0, 2.0 * (double)rand() / RAND_MAX - 1.0);
    circle->m = m;
    circle->maxa = maxa;
    circle->setProperty("shoot", "");
    circle->setProperty("health", health);
    circle->setProperty("fraction", FRACTION_ENEMY);
    circle->BFObject::setColor(colorlist[(*circle)["fraction"].toInt() % colorlistsize]);
    circle->setProperty("cooldown", cooldown);
    circle->setProperty("cooldowncount", 0.0);

    BFOColoredCircle *bullet = (BFOColoredCircle *)manager->getFactory()->newObject(typehash(BFOColoredCircle));
    //bullet->setColor(1.0, 0, 0, 1.0);
    bullet->r = bulletr;
    bullet->v = Vector2d(0, bulletv);
    bullet->m = bulletm;
    bullet->setProperty("isBullet", "Yes");
    bullet->setProperty("damage", damage);
    QBuffer bulletbuf;
    bulletbuf.open(QIODevice::ReadWrite);
    manager->getFactory()->encodeObject(bullet, &bulletbuf);
    bulletbuf.seek(0);
    manager->getFactory()->deleteObject(bullet);

    circle->setProperty("bullet prototype", bulletbuf.data());
    manager->insertObject(circle);

    controller = new BFCRandomShootDodge(manager, circle->getID());
    manager->registerController(controller);
}
