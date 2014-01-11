#ifndef BFCRANDOMSHOOTDODGE_H
#define BFCRANDOMSHOOTDODGE_H

#include "BFController.h"

class BFCRandomShootDodge : public BFController
{
public:
    BFCRandomShootDodge(BFManager *_manager, BFObject *_obj);
    virtual ~BFCRandomShootDodge();
    //virtual BFControllerType getType() const = 0;
    virtual void applyControl(); //apply control to the object

protected:
    void shoot(BFObject *aim);
    void shoot(double theta);
    void dodge(BFObject *bullet);
    double bulletv;

};

#endif // BFCRANDOMSHOOTDODGE_H
