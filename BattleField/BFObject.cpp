#include "BFObject.h"

long BFObject::count = 0;

BFObject::BFObject(BFManager *_manager)
    :manager(_manager)
{
    id = count;
    count++;
}

BFObject::~BFObject()
{
}

bool BFObject::intersectWith(BFObject *b)
{
    return manager->intersecting(this, b);
}
