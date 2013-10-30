#ifndef BFOCOLOREDCIRCLE_H
#define BFOCOLOREDCIRCLE_H

#include "BFOCircle.h"

class BFOColoredCircle : public BFOCircle
{
public:
    BFOColoredCircle(BFManager *manager);
    void draw(QGLWidget *glwidget);
    void onIntersection(BFObject *b, Vector2d impulse); //should be modified later
    double red, green, blue, alpha;
};

#endif // BFOCOLOREDCIRCLE_H
