#include <cmath>

#include "BFCAIRandom.h"

BFCAIRandom::BFCAIRandom(BFManager *_manager, BFObjectID _obj) :
    BFCRandomShootDodge(_manager, _obj)
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


std::vector<ControlEvent> &BFCAIRandom::getControl()
{
    controlevents.clear();
    obj = getObjectPointer();
    if (!obj)
        return controlevents;
    ControlEvent event(obj->getID());
    randomWalk(event);
    controlevents.push_back(event);
    return controlevents;
}
