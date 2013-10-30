#ifndef BFOCIRCLE_H
#define BFOCIRCLE_H

#include "BFObject.h"
#include "../BFManager.h"
#include "../Vector2d.h"

class BFOCircle : public BFObject
{
    friend class BFManager;

public:
    BFOCircle(BFManager *manager);
    virtual ~BFOCircle();
    virtual void draw(QGLWidget *glwidget);
    BFObjectType getType() const
    {
        return BFO_CIRCLE;
    }
    virtual void onIntersection(BFObject *b, Vector2d impulse) //should be modified later
    {}
    double getRoughRadius() const
    {
        return r;
    }
    Vector2d getPosition() const
    {
        return p;
    }

    void move(double time);

//private:
    Vector2d p, v, a; //position, velocity, acceleration
    double m, r; //mass, radius
    double maxa; //maximum acceleration

};

#endif // BFOCIRCLE_H
