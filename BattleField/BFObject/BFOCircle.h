#ifndef BFOCIRCLE_H
#define BFOCIRCLE_H

#include "BFObject.h"
#include "../BFManager.h"
#include "../Vector2d.h"

class BFOCircle : public BFObject
{
    friend class BFManager;

public:
    BFOCircle();
    //BFOCircle(BFManager *manager);
    virtual ~BFOCircle();
    virtual BFObject *newObject();
    virtual BFObject *duplicate();

    virtual void draw(QGLWidget *glwidget);
    virtual BFObjectType getType() const;
    BFObjectShape getShape() const;
    double getMass() const;
    //virtual void onIntersection(BFObject *b, Vector2d impulse) //should be modified later
    //{}
    double getRoughRadius() const;
    Vector2d getPosition() const;
    Vector2d getVelocity() const;
    void setVelocity(Vector2d vel);
    double getMaxAcceleration() const;
    void setAcceleration(Vector2d acc);

    void move(double time);

    virtual void encode(QIODevice *device); //must be called if overwritten by subclasses
    virtual void decode(QIODevice *device); //must be called if overwritten by subclasses

//private:
    Vector2d p, v, a; //position, velocity, acceleration
    double m, r; //mass, radius
    double maxa; //maximum acceleration

};

#endif // BFOCIRCLE_H
