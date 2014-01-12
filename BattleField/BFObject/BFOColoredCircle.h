#ifndef BFOCOLOREDCIRCLE_H
#define BFOCOLOREDCIRCLE_H

#include "BFOCircle.h"

class BFOColoredCircle : public BFOCircle
{
public:
    BFOColoredCircle();
    ~BFOColoredCircle();
    //BFOColoredCircle(BFManager *manager = 0);
    BFObject *newObject();
    BFObject *duplicate();

    void draw(QGLWidget *glwidget);
    virtual BFObjectType getType() const;
    void setColor(double r, double g, double b, double a);
    void setAlpha(double a);
    //const double *getColor() const;
    //void onIntersection(BFObject *b, Vector2d impulse); //should be modified later

    virtual void encode(QIODevice *device); //must be called if overwritten by subclasses
    virtual void decode(QIODevice *device); //must be called if overwritten by subclasses

    double c[4];
    double &red = c[0], &green = c[1], &blue = c[2], &alpha = c[3];
};

#endif // BFOCOLOREDCIRCLE_H
