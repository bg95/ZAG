#ifndef BFOCOLOREDCIRCLE_H
#define BFOCOLOREDCIRCLE_H

#include "BFOCircle.h"

class BFOColoredCircle : public BFOCircle
{
public:
    BFOColoredCircle();
    //BFOColoredCircle(BFManager *manager = 0);
    void draw(QGLWidget *glwidget);
    void setColor(double r, double g, double b, double a);
    const double *getColor() const;
    //void onIntersection(BFObject *b, Vector2d impulse); //should be modified later

    double c[4];
    double &red = c[0], &green = c[1], &blue = c[2], &alpha = c[3];
};

#endif // BFOCOLOREDCIRCLE_H
