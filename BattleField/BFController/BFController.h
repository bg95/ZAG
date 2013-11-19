#ifndef BFCONTROLLER_H
#define BFCONTROLLER_H

class BFObject;

enum BFControllerType
{
    BFC_NONE, BFC_HUMAN, BFC_AI, BFC_REMOTE
};

class BFController //A controller to control an object (abstract class)
{
public:
    BFController(BFObject *_obj);
    virtual ~BFController();
    virtual BFControllerType getType() const = 0;
    virtual void applyControl() = 0; //apply control to the object

protected:
    BFObject *obj;

};

#include "../BFObject/BFObject.h"

#endif // BFCONTROLLER_H
