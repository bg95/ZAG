#ifndef BFCRANDOMSHOOTDODGE_H
#define BFCRANDOMSHOOTDODGE_H

#include "BFController.h"

class BFCRandomShootDodge : public BFController
{
public:
    BFCRandomShootDodge(BFManager *_manager, BFObjectID _obj);
    virtual ~BFCRandomShootDodge();
    //virtual BFControllerType getType() const = 0;
    virtual void applyControl(); //apply control to the object

protected:
    void shoot(BFObject *aim);
    void shoot(double theta);
    void dodge(BFObject *bullet);
    void randomWalk();
    double bulletv;
    int count;
    double theta, a;
    BFObject *obj;

};

#endif // BFCRANDOMSHOOTDODGE_H
