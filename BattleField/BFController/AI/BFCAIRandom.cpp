#include <cmath>

#include "BFCAIRandom.h"

BFCAIRandom::BFCAIRandom(BFManager *_manager, BFObject *_obj) :
    BFCAI(_manager, _obj), obj(_obj)
{
    count = 0;
}

BFCAIRandom::~BFCAIRandom()
{
}

void BFCAIRandom::applyControl()
{
    const double PI = 3.14159265358979323846264338327950288419716939937510;
    if (obj->getType() == BFO_CIRCLE)
    {
        BFOCircle *cir = (BFOCircle *)obj;
        if (count <= 0)
        {
            theta = rand() * 2 * PI / (double)RAND_MAX;
            a = rand() * cir->maxa / (double)RAND_MAX;
            count = rand() & 255;
        }
        count--;
        cir->a = a * Vector2d(cos(theta), sin(theta));
    }
}
