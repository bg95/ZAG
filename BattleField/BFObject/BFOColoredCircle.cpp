#include <cmath>

#include "BFOColoredCircle.h"

BFOColoredCircle::BFOColoredCircle(BFManager *manager) :
    BFOCircle(manager)
{
    red = 1;
    green = 1;
    blue = 1;
    alpha = 1;
}

void BFOColoredCircle::draw(QGLWidget *glwidget)
{
    glwidget->makeCurrent();
    glColor4d(red, green, blue, alpha);/*
    red = 0.9 * red + 0.1;
    green = 0.9 * green + 0.1;
    blue = 0.9 * blue + 0.1;*/
    BFOCircle::draw(glwidget);
}

void BFOColoredCircle::setColor(double r, double g, double b, double a)
{
    red = r;
    green = g;
    blue = b;
    alpha = a;
}

const double *BFOColoredCircle::getColor() const
{
    return c;
}
/*
void BFOColoredCircle::onIntersection(BFObject *b0, Vector2d impulse)
{
    if (b0->getType() == BFO_CIRCLE)
    {
        BFOCircle *b = (BFOCircle *)b0;
        double t = 1.0 - atan2(impulse.abs(), m) / (3.141592653 / 2.0);
        blue *= t;
        green *= t;
        green *= 0.9;
    }
}
*/
