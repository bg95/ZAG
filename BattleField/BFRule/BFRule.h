#ifndef BFRULE_H
#define BFRULE_H

class BFManager;

enum BFRuleType
{
    BFR_Null, BFR_Collision
};

class BFRule //This class controls what to do when intersections / user inputs occur
{
public:
    explicit BFRule(BFManager *_manager);
    virtual ~BFRule();
    virtual BFRuleType getType() const; //deprecated
    virtual void processInput();
    virtual void filterIntersections();
    virtual void processIntersections();

protected:
    BFManager *manager;

};

#include "../BFManager.h"

#endif // BFRULE_H
