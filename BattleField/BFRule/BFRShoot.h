#ifndef BFRSHOOT_H
#define BFRSHOOT_H

#include "BFRule.h"

class BFRShoot : public BFRule
{
public:
    explicit BFRShoot(BFManager *_manager);
    virtual ~BFRShoot();
    virtual BFRuleType getType() const;
    virtual void filterIntersections();
    virtual void processIntersections();
    virtual void processInput();

protected:
    void processIntersection(BFOCircle *a, BFOCircle *b, double time);
    void processBoundaryIntersection(BFOCircle *a, IntersectionEvent::Boundary b, double time);
};

#endif // BFRSHOOT_H
