#include "BFOCircle.h"

BFOCircle::BFOCircle(BFManager *manager)
    :BFObject(manager)
{
}

BFOCircle::~BFOCircle()
{
}

void BFOCircle::draw(QPainter *painter)
{
    QRectF region(p.x - r, p.y - r, p.x + r, p.y + r);
    painter->drawEllipse(region);
}
