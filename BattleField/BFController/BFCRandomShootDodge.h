#ifndef BFCRANDOMSHOOTDODGE_H
#define BFCRANDOMSHOOTDODGE_H

#include "BFController.h"

class BFCRandomShootDodge : public BFController
{
public:
    BFCRandomShootDodge(BFManager *_manager, BFObjectID _obj);
    virtual ~BFCRandomShootDodge();
    //virtual BFControllerType getType() const = 0;
    virtual std::vector<ControlEvent> &getControl(); //get control events
    virtual void applyControl(); //apply control to the object

protected:
    void shoot(ControlEvent &event, BFObject *aim);
    void shoot(ControlEvent &event, double theta);
    void dodge(ControlEvent &event);
    void randomWalk(ControlEvent &event);
    double bulletv;
    int count;
    double theta, a;
    double dist;
    BFObject *obj;

private:
    Vector2d intersectionPosition(Vector2d p1, Vector2d p2, Vector2d v1, Vector2d v2);
};

#endif // BFCRANDOMSHOOTDODGE_H
