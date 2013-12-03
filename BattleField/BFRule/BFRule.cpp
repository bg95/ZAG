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

void BFRule::processIntersections()
{
}

void BFRule::processInput()
{
}
