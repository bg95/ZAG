#ifndef BFMANAGER_H
#define BFMANAGER_H

#include <set>

class BFObject;
class BFOCircle;

class BFManager
{
public:
    BFManager();
    bool intersecting(const BFObject *a, const BFObject *b);
    bool intersecting(BFOCircle *a, BFOCircle *b);

private:
    std::set<BFObject *> objects;

};

#include "BFObject.h"
#include "BFOCircle.h"

#endif // BFMANAGER_H
