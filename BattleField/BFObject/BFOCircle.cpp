#include <cmath>
#include <GL/gl.h>
#include "../Geometry.h"
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
    /*glBegin(GL_LINE_STRIP);
    for (double theta = 0; theta < 2 * 3.1415926 + 0.1; theta += 0.1)
        glVertex2d(p.x + r * cos(theta), p.y + r * sin(theta));
    glEnd();*/
    glPushMatrix();
        glTranslatef(p.x / 2.0, p.y / 2.0, 0.0);
        glutSolidCylinder(r / 2.0, 0.01, 16, 16);
    glPopMatrix();
}

BFObjectType BFOCircle::getType() const
{
    return BFO_CIRCLE;
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

    device->write((const char *)&p, sizeof(p));
    device->write((const char *)&v, sizeof(v));
    device->write((const char *)&a, sizeof(a));
    device->write((const char *)&m, sizeof(m));
    device->write((const char *)&r, sizeof(r));
    device->write((const char *)&maxa, sizeof(maxa));
}

void BFOCircle::decode(QIODevice *device)
{
    BFObject::decode(device);

    device->read((char *)&p, sizeof(p));
    device->read((char *)&v, sizeof(v));
    device->read((char *)&a, sizeof(a));
    device->read((char *)&m, sizeof(m));
    device->read((char *)&r, sizeof(r));
    device->read((char *)&maxa, sizeof(maxa));
}
