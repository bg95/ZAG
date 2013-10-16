#ifndef BFOCIRCLE_H
#define BFOCIRCLE_H

#include "BFObject.h"
#include "BFManager.h"
#include "Vector2d.h"

class BFOCircle : public BFObject
{
    friend class BFManager;

public:
    BFOCircle(BFManager *manager);
    ~BFOCircle();
    void draw(QPainter *painter);
    BFObjectType getType() const
    {
        return BFO_CIRCLE;
    }

private:
    Vector2d p, v, a; //position, velocity, acceleration
    double m, r; //mass, radius

};

#endif // BFOCIRCLE_H
