#include <cmath>

#include "BFOCircle.h"

BFOCircle::BFOCircle()
    :p(0, 0), v(0, 0), a(0, 0)
{
}
/*
BFOCircle::BFOCircle(BFManager *manager)
    :BFObject(manager), p(0, 0), v(0, 0), a(0, 0)
{
}
*/
BFOCircle::~BFOCircle()
{
}

BFObject *BFOCircle::newObject()
{
    return new BFOCircle;
}

BFObject *BFOCircle::duplicate()
{
    BFOCircle *ptr = (BFOCircle *)BFObject::duplicate();
    ptr->p = p;
    ptr->v = v;
    ptr->a = a;
    ptr->m = m;
    ptr->r = r;
    ptr->maxa = maxa;
    return ptr;
}

void BFOCircle::draw(QGLWidget *glwidget)
{
    //QRectF region(p.x - r, p.y - r, p.x + r, p.y + r);
    //painter->drawEllipse(region);
    //qDebug("drawing a circle");
    //glColor4d(1.0, 1.0, 1.0, 1.0);
    glwidget->makeCurrent();
    glBegin(GL_LINE_STRIP);
    for (double theta = 0; theta < 2 * 3.1415926 + 0.1; theta += 0.1)
        glVertex2d(p.x + r * cos(theta), p.y + r * sin(theta));
    glEnd();
}

BFObjectType BFOCircle::getType() const
{
    return typeid(BFOCircle).hash_code();
    //return BFO_CIRCLE;
}

BFObjectShape BFOCircle::getShape() const
{
    return BFO_CIRCULAR;
}

double BFOCircle::getRoughRadius() const
{
    return r;
}
Vector2d BFOCircle::getPosition() const
{
    return p;
}

void BFOCircle::move(double time)
{
    p = p + v * time + 0.5 * a * time * time;
    v = v + a * time;
}

void BFOCircle::encode(QIODevice *device)
{
    BFObject::encode(device);

    writeVector2d(device, p);
    writeVector2d(device, v);
    writeVector2d(device, a);
    device->write((const char *)&m, sizeof(m));
    device->write((const char *)&r, sizeof(r));
    device->write((const char *)&maxa, sizeof(maxa));
}

void BFOCircle::decode(QIODevice *device)
{
    BFObject::decode(device);

    readVector2d(device, p);
    readVector2d(device, v);
    readVector2d(device, a);
    device->read((char *)&m, sizeof(m));
    device->read((char *)&r, sizeof(r));
    device->read((char *)&maxa, sizeof(maxa));
}
