#ifndef BFRSHOOT_H
#define BFRSHOOT_H

#include "BFRule.h"

//Simple rule that allow shooting
class BFRShoot : public BFRule
{
public:
    static const double eta; //drag force = 6 * pi * eta * r * v

    explicit BFRShoot(BFManager *_manager);
    virtual ~BFRShoot();
    virtual BFRuleType getType() const;
    virtual void initialize();
    virtual void processInput();
    virtual void filterIntersections();
    virtual void processIntersections();

protected:
    virtual void shoot(BFObject *obj, double theta);
    void processIntersection(BFOCircle *a, BFOCircle *b, double time);
    void processBoundaryIntersection(BFOCircle *a, IntersectionEvent::Boundary b, double time);
};

#endif // BFRSHOOT_H
