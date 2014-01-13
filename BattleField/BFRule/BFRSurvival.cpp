#include "../BFObject/BFOColoredCircle.h"
#include "../BFController/BFCRandomShootDodge.h"
#include "../BFController/BFCHumanAndRSD.h"
#include "global.h"

#include <QBuffer>
#include <cstdlib>

#include "BFRSurvival.h"

BFRSurvival::BFRSurvival(BFManager *_manager):
    BFRShoot(_manager)
{
    counter = 0;
    difficulty = 0;
    strength = 0;
    generating = false;
}

BFRSurvival::~BFRSurvival()
{
    manager->getFactory()->deleteObject(bulletproto);
}

int BFRSurvival::getNumberFractions() const
{
    return 1;
}

void BFRSurvival::initialize()
{
    counter = 0;
    difficulty = 0;
    strength = 0;
    generateInitialObjects();
}

void BFRSurvival::processInput()
{
    BFRShoot::processInput();

    auto iter = manager->getObjects().begin();
    for (iter = manager->getObjects().begin(); iter != manager->getObjects().end(); iter++)
        if ((**iter)["fraction"] == FRACTION_ENEMY)
            break;
    if (iter == manager->getObjects().end() && !generating)
    {
        counter++;
        if (counter % 20 == 0)
        {
            generating = true;
            diffleft = difficulty;
            difficulty += 20.0;
            strength++;
            counter = 0;
        }
    }
    if (generating)
    {
        double num = (double)rand() / (double)RAND_MAX;
        num *= num;
        num = difficulty / (10 + 40 * num);
        generateObjectByStrength(num);
        diffleft -= num;
        if (diffleft <= 0.0)
            generating = false;
    }
}

void BFRSurvival::generateInitialObjects()
{
    qDebug("gen init");
    BFOColoredCircle *circle;
    std::vector<BFObjectID> objid;

    BFOColoredCircle *bullet = (BFOColoredCircle *)manager->getFactory()->newObject(typehash(BFOColoredCircle));
    bulletproto = bullet;

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
    //manager->getFactory()->deleteObject(bullet);
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

    circle->setProperty("isHumanControlled", true);
    mainobj = circle->getID();

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
}

void BFRSurvival::generateObjectByStrength(double str)
{
    str += 1.0;

    double r = sqrt((double)str) / 30.0;
    double m = 10.0 * r * r;
    double maxa = 0.5 / r;
    double health = m * 3;
    r /= 2.0;

    double bulletr = r / 5.0;
    double bulletm = 30.0 * bulletr * bulletr;
    double bulletv = 3;
    double damage = bulletm * 20.0;
    double cooldown = 0.5 / log(str);

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

void BFRSurvival::modifyStrength(int str)
{
    BFObject *obj = manager->getFactory()->objectByID(mainobj);
    if (!obj)
        return;
    (*obj)["health"] = (*obj)["health"].toDouble() + 1.0;
    (*obj)["cooldown"] = (*obj)["cooldown"].toDouble() / 1.01;

    //(*bulletproto)["damage"] = (*bulletproto)["damage"].toDouble() + 0.1;
    //((BFOColoredCircle *)bulletproto)->r *= 1.01;

    QBuffer bulletbuf;
    bulletbuf.open(QIODevice::ReadWrite);
    manager->getFactory()->encodeObject(bulletproto, &bulletbuf);
    bulletbuf.seek(0);
    obj->setProperty("bullet prototype", bulletbuf.data());
}
