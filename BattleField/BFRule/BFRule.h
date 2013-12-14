#ifndef BFRULE_H
#define BFRULE_H

#include "../BFManager.h"

enum BFRuleType
{
    BFR_Null, BFR_Collision
};

class BFRule //This class controls what to do when intersections / user inputs occur
{
public:
    explicit BFRule(BFManager *_manager);
    virtual ~BFRule();
    virtual BFRuleType getType() const;
    virtual void processIntersections();
    virtual void processInput();

protected:
    BFManager *manager;

};

#endif // BFRULE_H
