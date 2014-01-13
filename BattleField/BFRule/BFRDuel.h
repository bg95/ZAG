#ifndef BFRDUEL_H
#define BFRDUEL_H

#include "BFRShoot.h"

//Dueling between two fractions
class BFRDuel : public BFRShoot
{
public:
    explicit BFRDuel(BFManager *_manager);
    virtual ~BFRDuel();

    virtual int getNumberFractions() const;
    virtual void initialize();
    //virtual void processInput();

private:
    void generateFraction(int frac);

};

#endif // BFRDUEL_H
