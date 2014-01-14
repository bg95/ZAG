#include <QBuffer>
#include "../BFObject/BFOColoredCircle.h"
#include "../BFController/BFCHumanAndRSD.h"

#include "BFRDuel.h"

BFRDuel::BFRDuel(BFManager *_manager) :
    BFRShoot(_manager)
{
}

BFRDuel::~BFRDuel()
{
}

int BFRDuel::getNumberFractions() const
{
    return 2;
}

void BFRDuel::initialize()
{
    generateFraction(0);
    generateFraction(1);
    manager->getBattleField()->setFocusObject(manager->getObjectControlled());

    initializeColor();
}

void BFRDuel::generateFraction(int frac)
{
    qDebug("gen frac %d", frac);
    Vector2d fraccent = frac == 0 ? Vector2d(0, 0.9) : Vector2d(0, -0.9);
    BFOColoredCircle *circle;
    std::vector<BFObjectID> objid;

    BFOColoredCircle *bullet = (BFOColoredCircle *)manager->getFactory()->newObject(typehash(BFOColoredCircle));
    //bulletproto = bullet;

    bullet->setColor(1.0, 0, 0, 1.0);
    bullet->r = 0.01;
    bullet->v = Vector2d(0, 6);
    bullet->m = 0.01;
    bullet->setProperty("isBullet", "Yes");
    bullet->setProperty("damage", 0.05);
    QBuffer bulletbuf;
    bulletbuf.open(QIODevice::ReadWrite);
    manager->getFactory()->encodeObject(bullet, &bulletbuf);
    bulletbuf.seek(0);
    //manager->getFactory()->deleteObject(bullet);
    //shooter property is set in BFRShoot

    //circle = new BFOColoredCircle;//(manager);
    circle = (BFOColoredCircle *)manager->getFactory()->newObject(typehash(BFOColoredCircle));
    //bullet->setProperty("shooter", (unsigned long long)circle);
    circle->p = fraccent;
    circle->r = 0.1;
    circle->v = Vector2d(0, 0);
    circle->m = 1;
    circle->maxa = 5;
    circle->setColor(0.0, 0.5, 1.0, 1.0);
    circle->setProperty("shoot", "");
    circle->setProperty("bullet prototype", bulletbuf.data());
    circle->setProperty("cooldown", 0.05);
    circle->setProperty("cooldowncount", 0.0);
    circle->setProperty("health", 2.0);
    circle->setProperty("fraction", frac);

    circle->setProperty("isHumanControlled", true);
    //mainobj = circle->getID();
    qDebug("human controlled: %d", circle->getID());

    manager->insertObject(circle);
    objid.push_back(circle->getID());
/*
    if (frac == 0)
        manager->getBattleField()->setFocusObject(circle->getID());
*/
    QBuffer *buf = new QBuffer;
    BFFactory *fac = manager->getFactory();
    circle = (BFOColoredCircle *)manager->getFactory()->newObject(typehash(BFOColoredCircle));//(manager);
    circle->p = Vector2d(-0.7, 0.9);
    circle->r = 0.05;
    circle->v = Vector2d(0, 0);
    circle->m = 0.25;
    circle->maxa = 5;
    circle->setProperty("health", 1.0);
    //(*circle)["fraction"] = 2;
    circle->setProperty("fraction", frac);

    circle->setProperty("shoot", "");
    circle->setProperty("bullet prototype", bulletbuf.data());
    circle->setProperty("cooldown", 0.2);
    circle->setProperty("cooldowncount", 0.0);

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
        circle->p = Vector2d(i / 8.0 - 0.9, 0.0) + fraccent;
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
        circle->p = Vector2d(-(i / 8.0 - 0.9), 0.0) + fraccent;
        manager->insertObject(circle);
        objid.push_back(circle->getID());
        //controller = new BFCAIRandom(manager, circle->getID());
        //manager->registerController(controller);
        //circles[i] = circle;
    }
    buf->close();
    delete buf;

    if (frac == 0)
    {
        BFController *ctrl = new BFCHumanAndRSD(manager, objid);
        manager->registerController(ctrl);
    }
}
