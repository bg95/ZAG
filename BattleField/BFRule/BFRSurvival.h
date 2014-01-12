#ifndef BFRSURVIVAL_H
#define BFRSURVIVAL_H

#include "BFRShoot.h"

class BFRSurvival : public BFRShoot
{
public:
    explicit BFRSurvival(BFManager *_manager);
    virtual ~BFRSurvival();
    //virtual BFRuleType getType() const;
    virtual int getNumberFractions() const;
    virtual void processInput();
    //virtual void filterIntersections();
    //virtual void processIntersections();

protected:
    const int FRACTION_ENEMY = 16;
    void generateObjectByStrength(int str);
    int difficulty;
    int strength;
    int counter;

};

#endif // BFRSURVIVAL_H
