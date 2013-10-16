#include "BFManager.h"

BFManager::BFManager()
{
}

bool BFManager::intersecting(const BFObject *a, const BFObject *b)
{
    if (a->getType() == BFO_CIRCLE)
    {
        if (b->getType() == BFO_CIRCLE)
            return intersecting((BFOCircle *)a, (BFOCircle *)b);
    }
}

bool BFManager::intersecting(BFOCircle *a, BFOCircle *b)
{
    return (a->p - b->p).abs() < a->r + b->r;
}
