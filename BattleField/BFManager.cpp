#include <Qt>
#include "BFController/BFCHuman.h"

#include "BFManager.h"

/**/
#include "main.h"  //only for debug

int BFManager::process_independent_intersections = 10;
double BFManager::epsi = 0.01;

BFManager::BFManager() :
    qtree(this)
{
    /*
    left = -1.0;
    right = 1.0;
    bottom = -1.0;
    top = 1.0;
    */
    left = -2.0;
    right = 2.0;
    bottom = -2.0;
    top = 2.0;
}

BFManager::~BFManager()
{
    std::set<BFObject *>::iterator iter;
    for (iter = objects.begin(); iter != objects.end(); iter++)
        factory.deleteObject(*iter);
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

void BFManager::destructObject(BFObject *o)
{
    if (o->getController())
        destructController(o->getController());
    factory.deleteObject(o);
    removeObject(o);
}

void BFManager::clearObjects()
{
    objects.clear();
}

void BFManager::destructObjects()
{
    std::set<BFObject *>::iterator iter;
    for (iter = objects.begin(); iter != objects.end(); iter++)
        factory.deleteObject(*iter);
    clearObjects();
}

bool BFManager::registerController(BFController *c)
{
    return controllers.insert(c).second;
}

void BFManager::unregisterController(BFController *c)
{
    controllers.erase(c); //not necessarily correct
}

void BFManager::destructController(BFController *c)
{
    delete c;
    unregisterController(c);
}

void BFManager::clearControllers()
{
    controllers.clear();
}

void BFManager::destructControllers()
{
    std::set<BFController *>::iterator ctrliter;
    for (ctrliter = controllers.begin(); ctrliter != controllers.end(); ctrliter++)
        delete *ctrliter;
    clearControllers();
}

void BFManager::keyPressEvent(QKeyEvent *keyevent)
{
    //qDebug("key %d pressed", keyevent->key());
    keyspressed.insert((Qt::Key)keyevent->key());
}

void BFManager::keyReleaseEvent(QKeyEvent *keyevent)
{
    //qDebug("key %d released", keyevent->key());
    keyspressed.erase((Qt::Key)keyevent->key());
}

void BFManager::mouseEvent(Vector2d mousepos, Qt::MouseButtons mousebut)
{
    //qDebug("mouse at %lf,%lf", mousepos.x, mousepos.y);
    mouseposition = mousepos;
    mousebuttons = mousebut;
}

void BFManager::wheelEvent(QWheelEvent *wheelevent)
{

}

void BFManager::nextFrame(double deltatime)
{
    dt = deltatime;
    std::set<BFController *>::iterator ctrliter;
    for (ctrliter = controllers.begin(); ctrliter != controllers.end(); ctrliter++)
    {
        (*ctrliter)->setKeysAndMouse(keyspressed, mouseposition, mousebuttons);
    }
    std::set<BFObject *>::iterator iter;/*
    for (iter = objects.begin(); iter != objects.end(); iter++)
    {
        qDebug("object %lX (manager)", (unsigned long long)(*iter));
    }*/
    rule->processInput();

    BFOCircle *cir;
    /*
    qDebug("circle0 (%lf,%lf)", circles[0]->p.x, circles[0]->p.y);
    qDebug("circle1 (%lf,%lf)", circles[1]->p.x, circles[1]->p.y);
    /**/
    for (iter = objects.begin(); iter != objects.end(); iter++)
    {
        switch ((*iter)->getShape())
        {
        case BFO_CIRCULAR:
            cir = (BFOCircle *)(*iter);/*
            cir->p = cir->p + cir->v * dt + 0.5 * cir->a * dt * dt;
            cir->v = cir->v + cir->a * dt;*/
            //qDebug("nextframe< p=(%lf,%lf) v=(%lf,%lf) a=(%lf,%lf)", ((BFOCircle *)cir)->p.x, ((BFOCircle *)cir)->p.y, cir->v.x, cir->v.y, cir->a.x, cir->a.y);
            cir->move(dt);
            //qDebug("nextframe> p=(%lf,%lf) v=(%lf,%lf) a=(%lf,%lf)", ((BFOCircle *)cir)->p.x, ((BFOCircle *)cir)->p.y, cir->v.x, cir->v.y, cir->a.x, cir->a.y);
            break;
        }
    }
    /*
    qDebug("circle0 (%lf,%lf)", circles[0]->p.x, circles[0]->p.y);
    qDebug("circle1 (%lf,%lf)", circles[1]->p.x, circles[1]->p.y);
    /**/
    findAllIntersections();
    for (int i = 0; i != process_independent_intersections && intersections.size(); i++)
    {
        //qDebug("proc ind");
        processIndependentIntersections();
        findAllIntersections();
    }
    processAllIntersections();
}

void BFManager::paintAll(QGLWidget *glwidget)
{
    //qDebug("paintAll");
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

std::set<BFController *> &BFManager::getControllers()
{
    return controllers;
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

double BFManager::getDT()
{
    return dt;
}

BFFactory *BFManager::getFactory()
{
    return &factory;
}

void BFManager::encodeAllObjects(QIODevice *device)
{
    for (auto iter = objects.begin(); iter != objects.end(); iter++)
        factory.encodeObject(*iter, device);
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
    if (a->getShape() == BFO_CIRCULAR)
    {
        BFOCircle a1(*(BFOCircle *)a);
        a1.move(time);
        if (b->getShape() == BFO_CIRCULAR)
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
    if (a->getShape() == BFO_CIRCULAR)
    {
        if (b->getShape() == BFO_CIRCULAR)
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
    if (a->getShape() == BFO_CIRCULAR)
    {
        BFOCircle a1(*(BFOCircle *)a);
        a1.move(time);
        return intersectingBoundary(&a1, b);
    }
    return false; //...
}

bool BFManager::intersectingBoundary(const BFObject *a, IntersectionEvent::Boundary b) ////
{
    if (a->getShape() == BFO_CIRCULAR)
    {
        return intersectingBoundary((BFOCircle *)a, b);
    }
    //return IntersectionEvent::INVALID;
    return false; //...
}

bool BFManager::intersectingBoundary(BFOCircle *a, IntersectionEvent::Boundary b) ////
{
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
    double time;
    intersections.clear();
    std::set<BFObject *>::iterator iter;
/*
    std::set<BFObject *>::iterator iter2;
    for (iter = objects.begin(); iter != objects.end(); iter++)
        for (iter2 = iter, iter2++; iter2 != objects.end(); iter2++)
        {
            if (intersecting(*iter, *iter2))
                intersections.push_back(IntersectionEvent(*iter, *iter2, intersectingTime(*iter, *iter2)));
        }

    std::vector<IntersectionEvent> intersectionsqtree;
*/
    qtree.clear();
    for (iter = objects.begin(); iter != objects.end(); iter++)
    {
        //qDebug("manager insert (%lf, %lf)", (*iter)->getPosition().x, (*iter)->getPosition().y);
        qtree.insertObject(*iter);
    }
    qtree.build();
    //qtree.setOutput(intersectionsqtree);
    qtree.setOutput(intersections);
    qtree.queryAll();
    //intersections = intersectionsqtree;

    //std::sort(intersections.begin(), intersections.end());
    //std::sort(intersectionsqtree.begin(), intersectionsqtree.end());
    /*
    for (int j = 0; j < (int)intersections.size(); j++)
    {
        int i;
        for (i = 0; i < (int)intersectionsqtree.size(); i++)
            if ((intersections[j] == intersectionsqtree[i]))
            {
                break;
            }
        if (i >= intersectionsqtree.size())
        {
            qDebug("inconsistency!");
            qDebug("brute-force: %lX %lX %lf", intersections[j].obj1, intersections[j].obj2, intersections[j].time);
            //qDebug("quadtree:    %lX %lX %lf", intersectionsqtree[j].obj1, intersectionsqtree[j].obj2, intersectionsqtree[j].time);
            qtree.query(intersections[j].obj1 < intersections[j].obj2 ? intersections[j].obj1 : intersections[j].obj2);
        }
    }
*/
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
    }

    rule->filterIntersections();
    int n = 0;
    std::vector<IntersectionEvent>::iterator interiter;
    for (interiter = intersections.begin(); interiter != intersections.end(); interiter++)
    {
        if (!(*interiter).ignored)
        {
            intersections[n] = (*interiter);
            n++;
        }
    }
    intersections.resize(n);
    //qDebug("intersections: %d", intersections.size());
}

void BFManager::processAllIntersections()
{
    rule->processIntersections();
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
        if ((*iter).ignored)
        {
            continue;
        }
        if (!(*iter).boundary)
        {
            a = (*iter).obj1;
            b = (*iter).obj2;
            if (isintersected.find(a) == isintersected.end() && isintersected.find(b) == isintersected.end())
            {
                isintersected.insert(a);
                isintersected.insert(b);
                intersections[n] = (*iter);
                n++;
            }
        }
        else
        {
            a = (*iter).obj;
            if (isintersected.find(a) == isintersected.end())
            {
                isintersected.insert(a);
                intersections[n] = (*iter);
                n++;
            }
        }
    }
    isintersected.clear();
    intersections.resize(n);
    processAllIntersections();
}
