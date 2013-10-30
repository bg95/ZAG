#include <cmath>

#include "BFOCircle.h"

BFOCircle::BFOCircle(BFManager *manager)
    :BFObject(manager)
{
}

BFOCircle::~BFOCircle()
{
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

void BFOCircle::move(double time)
{
    p = p + v * time + 0.5 * a * time * time;
    v = v + a * time;
}
