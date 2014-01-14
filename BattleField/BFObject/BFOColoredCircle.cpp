#include <cmath>

#include "BFOColoredCircle.h"

BFOColoredCircle::BFOColoredCircle()
{
    red = 1;
    green = 1;
    blue = 1;
    alpha = 1;
    br = 1;
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

BFOColoredCircle::~BFOColoredCircle()
{
}

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
    glColor4d(red * br + (1 - red) * (1 - br), green * br + (1 - green) * (1 - br), blue * br + (1 - blue) * (1 - br), alpha);/*
    red = 0.9 * red + 0.1;
    green = 0.9 * green + 0.1;
    blue = 0.9 * blue + 0.1;*/
    BFOCircle::draw(glwidget);
}

BFObjectType BFOColoredCircle::getType() const
{
    return typeid(BFOColoredCircle).hash_code();
    //return BFO_COLORED_CIRCLE;
}

void BFOColoredCircle::setColor(double r, double g, double b, double a)
{
    red = r;
    green = g;
    blue = b;
    alpha = a;
}

void BFOColoredCircle::setColor(double _br)
{
    br = _br;
}

void BFOColoredCircle::setAlpha(double a)
{
    alpha = a;
}

/*
const double *BFOColoredCircle::getColor() const
{
    return c;
}
*/
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
