#ifndef BFCONTROLLER_H
#define BFCONTROLLER_H

class BFObject;

enum BFControllerType
{
    BFC_NONE, BFC_HUMAN, BFC_AI, BFC_REMOTE
};

class BFController
{
public:
    BFController(BFObject *_obj);
    virtual ~BFController();
    virtual BFControllerType getType() const = 0;
    virtual void applyControl() = 0;

protected:
    BFObject *obj;

};

#include "../BFObject/BFObject.h"

#endif // BFCONTROLLER_H
