#include "../BFObject/BFOCircle.h"
#include "../BFController/BFCHuman.h"

#include "BFRCollision.h"

BFRCollision::BFRCollision(BFManager *_manager) :
    BFRule(_manager)
{
}

void BFRCollision::processIntersections()
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
            if (a->getType() == BFO_CIRCLE)
            {
                if (b->getType() == BFO_CIRCLE)
                    processIntersection((BFOCircle *)a, (BFOCircle *)b, (*iter).time);
            }
        }
        else
        {
            a = (*iter).obj;
            if (a->getType() == BFO_CIRCLE)
            {
                processBoundaryIntersection((BFOCircle *)a, (*iter).b, (*iter).time);
            }
        }
    }
}

void BFRCollision::processInput()
{
    std::set<BFController*> &controllers = manager->getControllers();
    std::set<BFController *>::iterator ctrliter;
    for (ctrliter = controllers.begin(); ctrliter != controllers.end(); ctrliter++)
    {
        switch ((*ctrliter)->getType())
        {
        case BFC_HUMAN:
            BFCHuman *hum = (BFCHuman *)(*ctrliter);
            //hum->setKeysAndMouse(keyspressed, mouseposition, mousebuttons);
            hum->applyControl();
            break;
        }
    }
}

void BFRCollision::processIntersection(BFOCircle *a, BFOCircle *b, double time)
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

void BFRCollision::processBoundaryIntersection(BFOCircle *a, IntersectionEvent::Boundary b, double time)
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
