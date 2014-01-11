#include <cmath>

#include "BFCAIRandom.h"

BFCAIRandom::BFCAIRandom(BFManager *_manager, BFObjectID _obj) :
    BFController(_manager, _obj)
{
    count = 0;
}

BFCAIRandom::~BFCAIRandom()
{
}
/*
BFControllerType BFCAIRandom::getType() const
{
    return BFC_AI;//...
}
*/
void BFCAIRandom::applyControl()
{
    obj = getObjectPointer();
    if (!obj)
        return;
    if (obj->getShape() == BFO_CIRCULAR)
    {
        BFOCircle *cir = (BFOCircle *)obj;
        if (count <= 0)
        {
            theta = rand() * 2 * PI / (double)RAND_MAX;
            a = rand() * cir->maxa / (double)RAND_MAX;
            count = rand() & 255;
        }
        count--;
        cir->a = a * Vector2d(cos(theta), sin(theta));
    }
}
