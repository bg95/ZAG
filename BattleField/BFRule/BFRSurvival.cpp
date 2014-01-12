#include "BFRSurvival.h"

BFRSurvival::BFRSurvival(BFManager *_manager):
    BFRShoot(_manager)
{
}

BFRSurvival::~BFRSurvival()
{
}

void BFRSurvival::processInput()
{
    BFRShoot::processInput();
}
