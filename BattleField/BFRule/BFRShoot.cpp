#include "../BFObject/BFOCircle.h"
#include "../BFObject/BFOColoredCircle.h"
#include "../BFController/BFCHuman.h"
#include "../BFController/BFCAIRandom.h"

//#include "BFRCollision.h"
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <QByteArray>
#include <QBuffer>

#include "BFRShoot.h"

const double BFRShoot::eta = 1;

BFRShoot::BFRShoot(BFManager *_manager) :
    BFRule(_manager)
{
}

BFRShoot::~BFRShoot()
{
}

BFRuleType BFRShoot::getType() const
{
    return BFR_Collision; //...
}

void BFRShoot::filterIntersections()
{
    std::vector<IntersectionEvent> &intersections = manager->getIntersections();
    std::vector<IntersectionEvent>::iterator iter;
    BFObject *a, *b;
    for (iter = intersections.begin(); iter != intersections.end(); iter++)
    {
        if (!(*iter).boundary)
        {
            a = (*iter).obj1;
            b = (*iter).obj2;
            if ((a->getProperty("isBullet") == "Yes" && a->getProperty("shooter") == (unsigned long long)b) ||
                (b->getProperty("isBullet") == "Yes" && b->getProperty("shooter") == (unsigned long long)a) ||
                (a->getProperty("isBullet") == "Yes" && b->getProperty("isBullet") == "Yes" && a->getProperty("shooter") == b->getProperty("shooter")))
            {
                (*iter).ignored = true;
            }
        }
        else
        {
            ;
        }
    }
}

void BFRShoot::processIntersections()
{
    std::vector<IntersectionEvent> &intersections = manager->getIntersections();
    std::vector<IntersectionEvent>::iterator iter;
    BFObject *a, *b;
    for (iter = intersections.begin(); iter != intersections.end(); iter++)
    {
        if (!(*iter).boundary)
        {
            bool bullet = false, aisbullet, bisbullet;
            a = (*iter).obj1;
            b = (*iter).obj2;
            aisbullet = a->getProperty("isBullet") == "Yes";
            bisbullet = b->getProperty("isBullet") == "Yes";
            if (aisbullet && bisbullet)
            {
                manager->destructObject(a);
                manager->destructObject(b);
                continue;
            }
            if (bisbullet)
            {
                std::swap(a, b);
                std::swap(aisbullet, bisbullet);
            }
            if (aisbullet) //b is not a bullet
            {
                double newhealth = b->getProperty("health").toDouble() - a->getProperty("damage").toDouble();
                if (newhealth <= 0)
                    manager->destructObject(b);
                else
                {
                    b->setProperty("health", newhealth);
                    ((BFOColoredCircle *)b)->setColor(1.0, newhealth, newhealth, 1.0);
                    ((BFOCircle *)b)->v = (((BFOCircle *)b)->v * ((BFOCircle *)b)->m + ((BFOCircle *)a)->v * ((BFOCircle *)a)->m) / (((BFOCircle *)b)->m + ((BFOCircle *)a)->m);
                }
                manager->destructObject(a);
                continue;
            }
            if (a->getShape() == BFO_CIRCULAR)
            {
                if (b->getShape() == BFO_CIRCULAR)
                    processIntersection((BFOCircle *)a, (BFOCircle *)b, (*iter).time);
            }
        }
        else
        {
            a = (*iter).obj;
            if (a->getProperty("isBullet") == "Yes")
            {
                manager->destructObject(a);
                continue;
            }
            if (a->getShape() == BFO_CIRCULAR)
            {
                processBoundaryIntersection((BFOCircle *)a, (*iter).b, (*iter).time);
            }
        }
    }
}

void BFRShoot::processInput()
{
    std::set<BFObject *>::iterator iter;
    for (iter = manager->getObjects().begin(); iter != manager->getObjects().end(); iter++)
    {
        BFOCircle *cir = (BFOCircle *)(*iter);
        cir->a = Vector2d(0, 0);
    }
    BFRule::processInput();
    for (iter = manager->getObjects().begin(); iter != manager->getObjects().end(); iter++)
        if ((*iter)->getProperty("isBullet") != "Yes")
    {
        BFOCircle *cir = (BFOCircle *)(*iter);
        cir->a = cir->a + -6.0 * PI * eta * cir->r * cir->v / cir->m;
    }
    for (iter = manager->getObjects().begin(); iter != manager->getObjects().end(); iter++)
    {
        //qDebug("object %lX", (unsigned long long)(*iter));
        BFObject *const &obj = (*iter);
        if (obj->getProperty("cooldowncount").isValid())
        {
            double cooldowncount = obj->getProperty("cooldowncount").toDouble();
            //qDebug("cooldown = %lf/%lf dt=%lf", cooldowncount, cooldown, manager->getDT());
            cooldowncount -= manager->getDT();
            if (cooldowncount < 0)
                cooldowncount = 0;
            obj->setProperty("cooldowncount", cooldowncount);
        }
        QVariant vartheta = (*iter)->getProperty("shoot");
        if (vartheta.isValid())
        {
            bool ok;
            double theta = vartheta.toDouble(&ok);
            if (!ok)
                continue;
            /*theta += dtheta;*/
            shoot(*iter, theta);
        }
        (*iter)->setProperty("shoot", "");
    }
    static bool pspace = false;
    if (!pspace && (manager->getKeysPressed().find(Qt::Key_Space) != manager->getKeysPressed().end()))
    {
        BFOColoredCircle *circle;
        BFCAIRandom *controller;
        //circle = new BFOColoredCircle;//(bf->getManager());
        circle = (BFOColoredCircle *)manager->getFactory()->newObject(typehash(BFOColoredCircle));
        circle->p = Vector2d(2.0 * (double)rand() / RAND_MAX - 1.0, 2.0 * (double)rand() / RAND_MAX - 1.0);
        circle->r = 0.05;
        circle->v = Vector2d(2.0 * (double)rand() / RAND_MAX - 1.0, 2.0 * (double)rand() / RAND_MAX - 1.0);
        circle->m = 0.25;
        circle->maxa = 5;
        circle->setProperty("shoot", "");
        circle->setProperty("health", 1.0);
        manager->insertObject(circle);
        controller = new BFCAIRandom(manager, circle);
        manager->registerController(controller);
        //circles[i] = circle;
    }
    pspace = manager->getKeysPressed().find(Qt::Key_Space) != manager->getKeysPressed().end();
}

void BFRShoot::shoot(BFObject *obj, double theta)
{
    QByteArray bulletba = obj->getProperty("bullet prototype").toByteArray();
    QBuffer bulletbuf(&bulletba);
    bulletbuf.open(QIODevice::ReadOnly);

    double cooldown = obj->getProperty("cooldown").toDouble();
    double cooldowncount = obj->getProperty("cooldowncount").toDouble();

    if (cooldowncount <= 0.0)
        //for (double dtheta = -PI / 9.0; dtheta <= PI / 9.0 + 1E-7; dtheta += PI / 81.0)
    {
        double dtheta = 0.0;
        bulletbuf.seek(0);
        BFObject *newobj = manager->getFactory()->decodeNewObject(&bulletbuf);
        BFOColoredCircle *newcir = (BFOColoredCircle *)newobj; //...... don't know how to avoid this
        newcir->p = obj->getPosition();
        double vabs = newcir->v.abs();
        double ddtheta = (((double)rand() / RAND_MAX) - 0.5) * PI / 81.0;
        newcir->v = ((BFOCircle *)obj)->v + vabs * Vector2d(cos(theta + dtheta + ddtheta), sin(theta + dtheta + ddtheta));
        newcir->setProperty("isBullet", "Yes");
        newcir->setProperty("shooter", (unsigned long long)obj);
        manager->insertObject(newcir);
        obj->setProperty("cooldowncount", cooldown + cooldowncount); //reset cooldowncount
        ((BFOCircle *)obj)->v = ((BFOCircle *)obj)->v - newcir->v * newcir->m / ((BFOCircle *)obj)->m;
    }
}

void BFRShoot::processIntersection(BFOCircle *a, BFOCircle *b, double time)
{
    //qDebug("intersection time %lf", time);
    a->move(time);
    b->move(time);

    Vector2d e = a->p - b->p;
    e = e / e.abs();
    double A, B;
    A = a->v & e;
    B = b->v & e;
    Vector2d ar, br;
    ar = a->v - A * e;
    br = b->v - B * e;
    double Al, Bl;
    Al = (A * (a->m - b->m) + 2 * b->m * B) / (a->m + b->m);
    Bl = (B * (b->m - a->m) + 2 * a->m * A) / (a->m + b->m);
    Vector2d vat, vbt;
    vat = ar + Al * e;
    vbt = br + Bl * e;
    Vector2d impulse = a->m * (vat - a->v);
    a->v = vat;
    b->v = vbt;

    a->move(-time);
    b->move(-time);
    //a->onIntersection(b, impulse); //should be modified later
    //b->onIntersection(a, -impulse); //should be modified later
}

void BFRShoot::processBoundaryIntersection(BFOCircle *a, IntersectionEvent::Boundary b, double time)
{
    double left = manager->getLeft(), right = manager->getRight(), top = manager->getTop(), bottom = manager->getBottom();
    a->move(time);
    switch (b)
    {
    case IntersectionEvent::LEFT:
        a->p.x = 2.0 * (left + a->r) - a->p.x;
        a->v.x = -a->v.x;
        break;
    case IntersectionEvent::RIGHT:
        //qDebug("a->p.x = %lf -> %lf", a->p.x, 2.0 * (right - a->r) - a->p.x);
        a->p.x = 2.0 * (right - a->r) - a->p.x;
        a->v.x = -a->v.x;
        break;
    case IntersectionEvent::BOTTOM:
        a->p.y = 2.0 * (bottom + a->r) - a->p.y;
        a->v.y = -a->v.y;
        break;
    case IntersectionEvent::TOP:
        a->p.y = 2.0 * (top - a->r) - a->p.y;
        a->v.y = -a->v.y;
        break;
    default:
        break;
    }
    a->move(-time);
}
