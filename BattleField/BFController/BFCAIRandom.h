#ifndef BFCAIRANDOM_H
#define BFCAIRANDOM_H

#include "BFController.h"

class BFCAIRandom : public BFController
{
public:
    BFCAIRandom(BFManager *_manager, BFObjectID _obj);
    virtual ~BFCAIRandom();
    //virtual BFControllerType getType() const;
    virtual void applyControl(); //apply control to the object

private:
    BFObject *obj;
    int count;
    double theta, a;

};

#endif // BFCAIRANDOM_H
