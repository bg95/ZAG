#include "../BFObject/BFOColoredCircle.h"
#include "../BFController/BFCRandomShootDodge.h"
#include "global.h"

#include <QBuffer>

#include "BFRSurvival.h"

BFRSurvival::BFRSurvival(BFManager *_manager):
    BFRShoot(_manager)
{
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
    BFRShoot::processInput();
}

void BFRSurvival::generateObjectByStrength(int str)
{
    double r, m, maxa, health;
    double bulletr, bulletv, bulletm, damage, cooldown;

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
