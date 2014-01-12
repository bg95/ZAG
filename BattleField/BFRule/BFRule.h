#ifndef BFRULE_H
#define BFRULE_H

class BFManager;

enum BFRuleType
{
    BFR_Null, BFR_Collision
};

//This class controls what to do when intersections / user inputs occur
class BFRule
{
public:
    explicit BFRule(BFManager *_manager);
    virtual ~BFRule();
    virtual BFRuleType getType() const; //deprecated
    virtual int getNumberFractions() const;
    //This is called before the battle starts
    virtual void initialize();
    //Process user/AI inputs
    virtual void processInput();
    //Remove useless intersection events
    virtual void filterIntersections();
    //Process intersection events
    virtual void processIntersections();

protected:
    BFManager *manager;

};

#include "../BFManager.h"

#endif // BFRULE_H
