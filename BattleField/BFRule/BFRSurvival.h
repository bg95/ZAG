#ifndef BFRSURVIVAL_H
#define BFRSURVIVAL_H

#include "BFRShoot.h"

//Survival mode. Steadily generates enemy objects
class BFRSurvival : public BFRShoot
{
public:
    explicit BFRSurvival(BFManager *_manager);
    virtual ~BFRSurvival();
    //virtual BFRuleType getType() const;
    virtual int getNumberFractions() const;
    virtual void initialize();
    virtual void processInput();
    //virtual void filterIntersections();
    //virtual void processIntersections();

protected:
    const int FRACTION_ENEMY = 17;
    void generateInitialObjects();
    void generateObjectByStrength(double str);
    void modifyStrength(int str);
    int difficulty;
    double strength;
    int counter;
    BFObjectID mainobj;
    BFObject *bulletproto;
    bool generating;
    double diffleft;

};

#endif // BFRSURVIVAL_H
