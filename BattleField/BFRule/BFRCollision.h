#ifndef BFRCOLLISION_H
#define BFRCOLLISION_H

#include "BFRule.h"

class BFRCollision : public BFRule
{
public:
    explicit BFRCollision(BFManager *_manager);
    virtual void process();

protected:
    void processIntersection(BFOCircle *a, BFOCircle *b, double time);
    void processBoundaryIntersection(BFOCircle *a, IntersectionEvent::Boundary b, double time);

};

#endif // BFRCOLLISION_H
