#ifndef BFRSHOOT_H
#define BFRSHOOT_H

#include "BFRule.h"

class BFRShoot : public BFRule
{
public:
    static const double eta; //drag force = 6 * pi * eta * r * v

    explicit BFRShoot(BFManager *_manager);
    virtual ~BFRShoot();
    virtual BFRuleType getType() const;
    virtual void filterIntersections();
    virtual void processIntersections();
    virtual void processInput();

protected:
    virtual void shoot(BFObject *obj, double theta);
    void processIntersection(BFOCircle *a, BFOCircle *b, double time);
    void processBoundaryIntersection(BFOCircle *a, IntersectionEvent::Boundary b, double time);
};

#endif // BFRSHOOT_H
