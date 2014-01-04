#include "../BFObject/BFOCircle.h"
#include "../BFObject/BFOColoredCircle.h"
#include "../BFController/BFCHuman.h"
#include "../BFController/BFCAI.h"

//#include "BFRCollision.h"
#include <sstream>
#include <cmath>
#include <cstdlib>

#include "BFRShoot.h"

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
                (b->getProperty("isBullet") == "Yes" && b->getProperty("shooter") == (unsigned long long)a))
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
            if (a->getType() == BFO_CIRCLE)
            {
                if (b->getType() == BFO_CIRCLE)
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
            if (a->getType() == BFO_CIRCLE)
            {
                processBoundaryIntersection((BFOCircle *)a, (*iter).b, (*iter).time);
            }
        }
    }
}

void BFRShoot::processInput()
{
    BFRule::processInput();
    std::set<BFObject *>::iterator iter;
    for (iter = manager->getObjects().begin(); iter != manager->getObjects().end(); iter++)
    {
        //qDebug("object %lX", (unsigned long long)(*iter));
        QVariant vartheta = (*iter)->getProperty("shoot");
        if (vartheta.isValid())
        {
            bool ok;
            double theta = vartheta.toDouble(&ok);
            if (!ok)
                continue;
            unsigned long long ptr = (*iter)->getProperty("bullet prototype").toULongLong();
            if (!ptr)
                continue;
            double cooldown = (*iter)->getProperty("cooldown").toDouble();
            double cooldowncount = (*iter)->getProperty("cooldowncount").toDouble();
            //qDebug("cooldown = %lf/%lf dt=%lf", cooldowncount, cooldown, manager->getDT());
            cooldowncount -= manager->getDT();
            (*iter)->setProperty("cooldowncount", cooldowncount);
            if (cooldowncount <= 0.0)
            {
                BFObject *newobj = ((BFObject *)ptr)->duplicate();
                BFOColoredCircle *newcir = (BFOColoredCircle *)newobj; //...... don't know how to avoid this
                newcir->p = (*iter)->getPosition();
                double vabs = newcir->v.abs();
                newcir->v = vabs * Vector2d(cos(theta), sin(theta));
                newcir->setProperty("isBullet", "Yes");
                newcir->setProperty("shooter", (unsigned long long)(*iter));
                manager->insertObject(newcir);
                //qDebug("Creating bullet");
                (*iter)->setProperty("cooldowncount", cooldown); //reset cooldowncount
            }
        }
        (*iter)->setProperty("shoot", "");
    }
    static bool pspace = false;
    if (!pspace && (manager->getKeysPressed().find(Qt::Key_Space) != manager->getKeysPressed().end()))
    {
        BFOColoredCircle *circle;
        circle = new BFOColoredCircle;//(bf->getManager());
        circle->p = Vector2d(2.0 * (double)rand() / RAND_MAX - 1.0, 2.0 * (double)rand() / RAND_MAX - 1.0);
        circle->r = 0.05;
        circle->v = Vector2d(2.0 * (double)rand() / RAND_MAX - 1.0, 2.0 * (double)rand() / RAND_MAX - 1.0);
        circle->m = 0.25;
        circle->setProperty("shoot", "");
        circle->setProperty("health", 1.0);
        manager->insertObject(circle);
        //circles[i] = circle;
    }
    pspace = manager->getKeysPressed().find(Qt::Key_Space) != manager->getKeysPressed().end();
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
