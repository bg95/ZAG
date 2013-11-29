#ifndef BFRULE_H
#define BFRULE_H

#include "../BFManager.h"

class BFRule //This class controls what to do when intersections / user inputs occur
{
public:
    explicit BFRule(BFManager *_manager);
    virtual void processIntersections();
    virtual void processInput();

protected:
    BFManager *manager;

};

#endif // BFRULE_H
