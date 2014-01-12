#include "BFRule.h"

BFRule::BFRule(BFManager *_manager) :
    manager(_manager)
{
}

BFRule::~BFRule()
{
}

BFRuleType BFRule::getType() const
{
    return BFR_Null;
}

void BFRule::processInput()
{
    std::set<BFController*> &controllers = manager->getControllers();
    std::set<BFController *>::iterator ctrliter;
    for (ctrliter = controllers.begin(); ctrliter != controllers.end(); ctrliter++)
    {
        //(*ctrliter)->applyControl();
        manager->applyControlEvents((*ctrliter)->getControl());
    }
}

void BFRule::filterIntersections()
{
}

void BFRule::processIntersections()
{
}
