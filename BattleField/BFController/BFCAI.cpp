#include "BFCAI.h"

BFCAI::BFCAI(BFManager *_manager, BFObject *_obj) :
    BFController(_obj), manager(_manager)
{
}

BFCAI::~BFCAI()
{
}

BFControllerType BFCAI::getType() const
{
    return BFC_AI;
}
