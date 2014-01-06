#include <cmath>

#include "BFOColoredCircle.h"

BFOColoredCircle::BFOColoredCircle()
{
    red = 1;
    green = 1;
    blue = 1;
    alpha = 1;
}
/*
BFOColoredCircle::BFOColoredCircle(BFManager *manager) :
    BFOCircle(manager)
{
    red = 1;
    green = 1;
    blue = 1;
    alpha = 1;
}
*/

BFObject *BFOColoredCircle::newObject()
{
    return new BFOColoredCircle;
}

BFObject *BFOColoredCircle::duplicate()
{
    BFOColoredCircle *ptr = (BFOColoredCircle *)BFOCircle::duplicate();
    memcpy(ptr->c, c, sizeof(c));
    return ptr;
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

BFObjectType BFOColoredCircle::getType() const
{
    return BFO_COLORED_CIRCLE;
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

void BFOColoredCircle::encode(QIODevice *device)
{
    BFOCircle::encode(device);

    device->write((const char *)c, sizeof(c));
}

void BFOColoredCircle::decode(QIODevice *device)
{
    BFOCircle::decode(device);

    device->read((char *)c, sizeof(c));
}
