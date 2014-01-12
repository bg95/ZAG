#ifndef BFRSURVIVAL_H
#define BFRSURVIVAL_H

#include "BFRShoot.h"

class BFRSurvival : public BFRShoot
{
public:
    explicit BFRSurvival(BFManager *_manager);
    virtual ~BFRSurvival();
    //virtual BFRuleType getType() const;
    virtual void processInput();
    //virtual void filterIntersections();
    //virtual void processIntersections();

protected:
    int difficulty;
    int strength;

};

#endif // BFRSURVIVAL_H
