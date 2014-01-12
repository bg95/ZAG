#ifndef BFCAIRANDOM_H
#define BFCAIRANDOM_H

#include "BFCRandomShootDodge.h"

class BFCAIRandom : public BFCRandomShootDodge
{
public:
    BFCAIRandom(BFManager *_manager, BFObjectID _obj);
    BFCAIRandom(BFManager *_manager, std::vector<BFObjectID> _obj);
    virtual ~BFCAIRandom();
    //virtual BFControllerType getType() const;
    virtual std::vector<ControlEvent> &getControl();
    //virtual void applyControl(); //apply control to the object

};

#endif // BFCAIRANDOM_H
