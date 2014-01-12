#ifndef BFCRANDOMSHOOTDODGE_H
#define BFCRANDOMSHOOTDODGE_H

#include "BFController.h"

class BFCRandomShootDodge : public BFController
{
public:
    BFCRandomShootDodge(BFManager *_manager, BFObjectID _obj);
    BFCRandomShootDodge(BFManager *_manager, std::vector<BFObjectID> _obj);
    virtual ~BFCRandomShootDodge();
    //virtual BFControllerType getType() const = 0;
    virtual std::vector<ControlEvent> &getControl(); //get control events
    virtual void applyControl(); //apply control to the object

protected:
    void shoot(ControlEvent &event, BFObject *aim);
    void shoot(ControlEvent &event, double theta);
    void dodge(ControlEvent &event, BFObject *bullet);
    void randomWalk(ControlEvent &event);
    double bulletv;
    int count;
    double theta, a;
    BFObject *obj;

private:
    void init(BFObjectID _obj);

};

#endif // BFCRANDOMSHOOTDODGE_H
