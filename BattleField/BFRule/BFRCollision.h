#ifndef BFRCOLLISION_H
#define BFRCOLLISION_H

#include "BFRule.h"
#include "../BFController/BFController.h"

class BFRCollision : public BFRule //Simple rigid body collision & keyboard control
{
public:
    explicit BFRCollision(BFManager *_manager);
    virtual void processIntersections();
    virtual void processInput();

protected:
    void processIntersection(BFOCircle *a, BFOCircle *b, double time);
    void processBoundaryIntersection(BFOCircle *a, IntersectionEvent::Boundary b, double time);

};

#endif // BFRCOLLISION_H
