#ifndef BFCAIRANDOM_H
#define BFCAIRANDOM_H

#include "../BFCAI.h"

class BFCAIRandom : public BFCAI
{
public:
    BFCAIRandom(BFManager *_manager, BFObject *_obj);
    virtual ~BFCAIRandom();
    virtual void applyControl(); //apply control to the object

private:
    BFObject *obj;
    int count;
    double theta, a;

};

#endif // BFCAIRANDOM_H
