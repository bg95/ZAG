#ifndef BFCAI_H
#define BFCAI_H

#include "BFController.h"

class BFCAI : public BFController
{
public:
    BFCAI(BFManager *_manager, BFObject *_obj);
    virtual ~BFCAI();
    BFControllerType getType() const;
    virtual void applyControl() = 0; //apply control to the object

protected:
    BFManager *manager;

};

#endif // BFCAI_H
