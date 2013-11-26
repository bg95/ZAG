#include <Qt>
#include "BFController/BFCHuman.h"

#include "BFManager.h"

double BFManager::epsi = 0.01;

BFManager::BFManager() :
    qtree(this)
{
    left = -1.0;
    right = 1.0;
    bottom = -1.0;
    top = 1.0;
}

BFManager::~BFManager()
{
    std::set<BFObject *>::iterator iter;
    for (iter = objects.begin(); iter != objects.end(); iter++)
        delete (*iter);
}

bool BFManager::insertObject(BFObject *o)
{
    std::pair<std::set<BFObject *>::iterator, bool> pair = objects.insert(o);
    return pair.second;
}

void BFManager::removeObject(BFObject *o)
{
    objects.erase(o); //not necessarily correct
}

bool BFManager::registerController(BFController *c)
{
    return controllers.insert(c).second;
}

void BFManager::unregisterController(BFController *c)
{
    controllers.erase(c); //not necessarily correct
}

void BFManager::keyPressEvent(QKeyEvent *keyevent)
{
    qDebug("key %d pressed", keyevent->key());
    keyspressed.insert((Qt::Key)keyevent->key());
}

void BFManager::keyReleaseEvent(QKeyEvent *keyevent)
{
    qDebug("key %d released", keyevent->key());
    keyspressed.erase((Qt::Key)keyevent->key());
}

void BFManager::mouseEvent(Vector2d mousepos, Qt::MouseButtons mousebut)
{
    qDebug("mouse at %lf,%lf", mousepos.x, mousepos.y);
    mouseposition = mousepos;
    mousebuttons = mousebut;
}

void BFManager::nextFrame(double deltatime)
{
    dt = deltatime;
    std::set<BFController *>::iterator ctrliter;
    for (ctrliter = controllers.begin(); ctrliter != controllers.end(); ctrliter++)
    {
        switch ((*ctrliter)->getType())
        {
        case BFC_HUMAN:
            BFCHuman *hum = (BFCHuman *)(*ctrliter);
            hum->setKeysAndMouse(keyspressed, mouseposition, mousebuttons);
            hum->applyControl();
            break;
        }
    }
    std::set<BFObject *>::iterator iter;
    BFOCircle *cir;
    for (iter = objects.begin(); iter != objects.end(); iter++)
    {
        switch ((*iter)->getType())
        {
        case BFO_CIRCLE:
            cir = (BFOCircle *)(*iter);/*
            cir->p = cir->p + cir->v * dt + 0.5 * cir->a * dt * dt;
            cir->v = cir->v + cir->a * dt;*/
            cir->move(dt);
            //qDebug("nextframe p=(%lf,%lf) v=(%lf,%lf) a=(%lf,%lf)", ((BFOCircle *)cir)->p.x, ((BFOCircle *)cir)->p.y, cir->v.x, cir->v.y, cir->a.x, cir->a.y);
            break;
        }
    }
    findAllIntersections();
    while (intersections.size())
    {
        processIndependentIntersections();
        findAllIntersections();
    }
    //processAllIntersections();
}

void BFManager::paintAll(QGLWidget *glwidget)
{
    std::set<BFObject *>::iterator iter;
    for (iter = objects.begin(); iter != objects.end(); iter++)
        (*iter)->draw(glwidget);
}

//interface for BFRule
void BFManager::setRule(BFRule *_rule)
{
    rule = _rule;
}

std::vector<IntersectionEvent> &BFManager::getIntersections()
{
    return intersections;
}

std::set<BFObject *> &BFManager::getObjects()
{
    return objects;
}

std::set<Qt::Key> &BFManager::getKeysPressed()
{
    return keyspressed;
}

Vector2d BFManager::getMousePosition()
{
    return mouseposition;
}

Qt::MouseButtons BFManager::getMouseButtons()
{
    return mousebuttons;
}

///intersection between objects
double BFManager::intersectingTime(const BFObject *a, const BFObject *b)
{
    double l = -dt, r = 0.0, m;
    double e = dt * epsi;
    while (r - l > e)
    {
        m = (l + r) / 2.0;
        if (intersectingBackTrace(a, b, m))
            r = m;
        else
            l = m;
    }
    return (l + r) / 2.0;
}

bool BFManager::intersectingBackTrace(const BFObject *a, const BFObject *b, double time)
{
    if (a->getType() == BFO_CIRCLE)
    {
        BFOCircle a1(*(BFOCircle *)a);
        a1.move(time);
        if (b->getType() == BFO_CIRCLE)
        {
            BFOCircle b1(*(BFOCircle *)b);
            b1.move(time);
            return intersecting(&a1, &b1);
        }
    }
    return false; //...
}

bool BFManager::intersecting(const BFObject *a, const BFObject *b)
{
    if (a->getType() == BFO_CIRCLE)
    {
        if (b->getType() == BFO_CIRCLE)
            return intersecting((BFOCircle *)a, (BFOCircle *)b);
    }
    return false;
}

bool BFManager::intersecting(BFOCircle *a, BFOCircle *b)
{
    double d = (a->p - b->p).abs();
    double dr = a->r > b->r ? a->r - b->r : b->r - a->r;
    return d < a->r + b->r;// && d > dr;
}

///intersection with boundary
double BFManager::intersectingBoundaryTime(const BFObject *a, IntersectionEvent::Boundary b)
{
    double l = -dt, r = 0.0, m;
    double e = dt * epsi;
    while (r - l > e)
    {
        m = (l + r) / 2.0;
        if (intersectingBoundaryBackTrace(a, b, m))
            r = m;
        else
            l = m;
    }
    return (l + r) / 2.0;
}

bool BFManager::intersectingBoundaryBackTrace(const BFObject *a, IntersectionEvent::Boundary b, double time)
{
    if (a->getType() == BFO_CIRCLE)
    {
        BFOCircle a1(*(BFOCircle *)a);
        a1.move(time);
        return intersectingBoundary(&a1, b);
    }
    return false; //...
}

bool BFManager::intersectingBoundary(const BFObject *a, IntersectionEvent::Boundary b) ////
{
    if (a->getType() == BFO_CIRCLE)
    {
        return intersectingBoundary((BFOCircle *)a, b);
    }
    //return IntersectionEvent::INVALID;
    return false; //...
}

bool BFManager::intersectingBoundary(BFOCircle *a, IntersectionEvent::Boundary b) ////
{
    /*
    if (a->p.x - a->r < left)
        return IntersectionEvent::LEFT;
    if (a->p.x + a->r > right)
        return IntersectionEvent::RIGHT;
    if (a->p.y - a->r < bottom)
        return IntersectionEvent::BOTTOM;
    if (a->p.y + a->r > top)
        return IntersectionEvent::TOP;
    return IntersectionEvent::NONE;
    */
    switch (b)
    {
    case IntersectionEvent::LEFT:
        return a->p.x - a->r < left;// ? -0.0 : IntersectionEvent::INVALID;
        break;
    case IntersectionEvent::RIGHT:
        return a->p.x + a->r > right;// ? -0.0 : IntersectionEvent::INVALID;
        break;
    case IntersectionEvent::BOTTOM:
        return a->p.y - a->r < bottom;// ? -0.0 : IntersectionEvent::INVALID;
        break;
    case IntersectionEvent::TOP:
        return a->p.y + a->r > top;// ? -0.0 : IntersectionEvent::INVALID;
        break;
    default:
        break;
    }
    return false;
}

void BFManager::findAllIntersections()
{
    //brute force
    double time;
    intersections.clear();
    std::set<BFObject *>::iterator iter;/*
    std::set<BFObject *>::iterator iter2;
    for (iter = objects.begin(); iter != objects.end(); iter++)
        for (iter2 = iter, iter2++; iter2 != objects.end(); iter2++)
        {
            if (intersecting(*iter, *iter2))
                intersections.push_back(IntersectionEvent(*iter, *iter2, intersectingTime(*iter, *iter2)));
        }*/
    //IntersectionEvent::Boundary b;

    qtree.clear();
    for (iter = objects.begin(); iter != objects.end(); iter++)
    {
        //qDebug("manager insert (%lf, %lf)", (*iter)->getPosition().x, (*iter)->getPosition().y);
        qtree.insertObject(*iter);
    }
    qtree.build();
    qtree.setOutput(intersections);
    qtree.queryAll();
    for (iter = objects.begin(); iter != objects.end(); iter++)
    {
        if (intersectingBoundary(*iter, IntersectionEvent::LEFT))
            intersections.push_back(IntersectionEvent(*iter, IntersectionEvent::LEFT, intersectingBoundaryTime(*iter, IntersectionEvent::LEFT)));
        if (intersectingBoundary(*iter, IntersectionEvent::RIGHT))
            intersections.push_back(IntersectionEvent(*iter, IntersectionEvent::RIGHT, intersectingBoundaryTime(*iter, IntersectionEvent::RIGHT)));
        if (intersectingBoundary(*iter, IntersectionEvent::BOTTOM))
            intersections.push_back(IntersectionEvent(*iter, IntersectionEvent::BOTTOM, intersectingBoundaryTime(*iter, IntersectionEvent::BOTTOM)));
        if (intersectingBoundary(*iter, IntersectionEvent::TOP))
            intersections.push_back(IntersectionEvent(*iter, IntersectionEvent::TOP, intersectingBoundaryTime(*iter, IntersectionEvent::TOP)));
       /*
        time = intersectingBoundary(*iter, IntersectionEvent::LEFT);
        if (time <= 0.0)
            intersections.push_back(IntersectionEvent(*iter, IntersectionEvent::LEFT, time));
        time = intersectingBoundary(*iter, IntersectionEvent::RIGHT);
        if (time <= 0.0)
            intersections.push_back(IntersectionEvent(*iter, IntersectionEvent::RIGHT, time));
        time = intersectingBoundary(*iter, IntersectionEvent::BOTTOM);
        if (time <= 0.0)
            intersections.push_back(IntersectionEvent(*iter, IntersectionEvent::BOTTOM, time));
        time = intersectingBoundary(*iter, IntersectionEvent::TOP);
        if (time <= 0.0)
            intersections.push_back(IntersectionEvent(*iter, IntersectionEvent::TOP, time));
        */
    }
    qDebug("intersections: %d", intersections.size());
}

void BFManager::processAllIntersections()
{/*
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
    }*/
    rule->process();
}

void BFManager::processIndependentIntersections()
{
    std::vector<IntersectionEvent>::iterator iter;
    BFObject *a, *b;
    int n;
    std::sort(intersections.begin(), intersections.end());
    isintersected.clear();
    n = 0;
    for (iter = intersections.begin(); iter != intersections.end(); iter++)
    {
        if (!(*iter).boundary)
        {
            a = (*iter).obj1;
            b = (*iter).obj2;
            if (isintersected.find(a) == isintersected.end() && isintersected.find(b) == isintersected.end())
            {
                isintersected.insert(a);
                isintersected.insert(b);/*
                if (a->getType() == BFO_CIRCLE)
                {
                    if (b->getType() == BFO_CIRCLE)
                        processIntersection((BFOCircle *)a, (BFOCircle *)b, (*iter).time);
                }*/
                intersections[n] = (*iter);
                n++;
            }
        }
        else
        {
            a = (*iter).obj;
            if (isintersected.find(a) == isintersected.end())
            {
                isintersected.insert(a);/*
                if (a->getType() == BFO_CIRCLE)
                {
                    processBoundaryIntersection((BFOCircle *)a, (*iter).b, (*iter).time);
                }*/
                intersections[n] = (*iter);
                n++;
            }
        }
    }
    isintersected.clear();
    intersections.resize(n);
    processAllIntersections();
}
/*
void BFManager::processIntersection(BFOCircle *a, BFOCircle *b, double time)
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
    a->onIntersection(b, impulse); //should be modified later
    b->onIntersection(a, -impulse); //should be modified later
}

void BFManager::processBoundaryIntersection(BFOCircle *a, IntersectionEvent::Boundary b, double time)
{
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
*/
