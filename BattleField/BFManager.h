#ifndef BFMANAGER_H
#define BFMANAGER_H


class BFObject;
class BFOCircle;
class IntersectionEvent;
class BFController;
class BFRule;
class BattleField;

#include <Qt>
#include <QKeyEvent>
#include <QMouseEvent>
#include <set>
#include <QPainter>
#include <QGLWidget>
#include "IntersectionEvent.h"
#include "QuadTree.h"
#include "Vector2d.h"
#include "BFFactory.h"

class BFManager
{
public:
    static int process_independent_intersections;
    static double epsi;

    BFManager(BattleField *bf);
    ~BFManager();

    BattleField *getBattleField();
    //Range of the battlefield
    double getLeft() const {return left;}
    double getRight() const {return right;}
    double getBottom() const {return bottom;}
    double getTop() const {return top;}

    //Object operations
    bool insertObject(BFObject *o);
    void removeObject(BFObject *o);
    void destructObject(BFObject *o);
    void clearObjects();
    void destructObjects();

    //Controller operations
    bool registerController(BFController *c);
    void unregisterController(BFController *c);
    void destructController(BFController *c);
    void clearControllers();
    void destructControllers();

    //ControlEvent processing
    void applyControlEvents(std::vector<ControlEvent> &events);

    //User Events
    void keyPressEvent(QKeyEvent *keyevent);
    void keyReleaseEvent(QKeyEvent *keyevent);
    void mouseEvent(Vector2d mousepos, Qt::MouseButtons mousebut);
    void wheelEvent(QWheelEvent* wheelevent);

    //Process user inputs, calculate positions of objects in the next frame, apply the rule
    void setDT(double deltatime);
    void processInput();
    void nextFrame();

    //Paint the background and all objects
    void paintAll(QGLWidget *glwidget);//(QPainter *painter);

    //Interface for BFRule
    void setRule(BFRule *_rule);
    std::vector<IntersectionEvent> &getIntersections();
    std::set<BFObject *> &getObjects();
    std::set<BFController *> &getControllers();
    std::set<Qt::Key> &getKeysPressed();
    Vector2d getMousePosition();
    Qt::MouseButtons getMouseButtons();
    double getDT();

    //Factory operations
    BFFactory *getFactory();
    void encodeAllObjects(QIODevice *device);
    void decodeNewAllObjects(QIODevice *device);
    void decodeReplaceAllObjects(QIODevice *device);

    //For randomly placing objects
    bool intersectingWithAnyObject(const BFObject *a);

    //Calculate intersections
    double intersectingTime(const BFObject *a, const BFObject *b);
    bool intersectingBackTrace(const BFObject *a, const BFObject *b, double time);
    bool intersecting(const BFObject *a, const BFObject *b);
    bool intersecting(BFOCircle *a, BFOCircle *b);

    double intersectingBoundaryTime(const BFObject *a, IntersectionEvent::Boundary b);
    bool intersectingBoundaryBackTrace(const BFObject *a, IntersectionEvent::Boundary b, double time);
    bool intersectingBoundary(const BFObject *a, IntersectionEvent::Boundary b);
    bool intersectingBoundary(BFOCircle *a, IntersectionEvent::Boundary b);

private:
    BattleField *battlefield;
    double left, right, bottom, top;
    std::set<BFObject *> objects;
    std::vector<IntersectionEvent> intersections;
    std::set<BFObject *> isintersected;
    double dt;

    std::set<BFController *> controllers;
    std::set<Qt::Key> keyspressed;
    Vector2d mouseposition;
    Qt::MouseButtons mousebuttons;
    QuadTree qtree;

    BFRule *rule;

    BFFactory factory;

    void findAllIntersections();
    void processAllIntersections();
    void processIndependentIntersections();
    //void processIntersection(BFOCircle *a, BFOCircle *b, double time);
    //void processBoundaryIntersection(BFOCircle *a, IntersectionEvent::Boundary b, double time);

};

#include "BattleField.h"
#include "BFObject/BFObject.h"
#include "BFObject/BFOCircle.h"
#include "BFController/BFController.h"
#include "BFRule/BFRule.h"

#endif // BFMANAGER_H
