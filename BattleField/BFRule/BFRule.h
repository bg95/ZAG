#ifndef BFRULE_H
#define BFRULE_H

#include "../BFManager.h"

class BFRule
{
public:
    explicit BFRule(BFManager *_manager);
    virtual void process();
    virtual void processInput();

protected:
    BFManager *manager;

};

#endif // BFRULE_H
