#include <QPainter>
#include "BattleField.h"

int BattleField::refresh_interval = 5;
int BattleField::display_refreshes = 6;


BattleField::BattleField(QWidget *parent) :
    QGLWidget(parent),
    refreshtimer(this),
    display_counter(0)
{
    refreshtimer.setInterval(refresh_interval);
    connect(&refreshtimer, SIGNAL(timeout()), this, SLOT(refresh()));
    refreshtimer.start();

    setMouseTracking(true);
    grabKeyboard();
    grabMouse();
}

bool BattleField::insertObject(BFObject *o)
{
    return manager.insertObject(o);
}

void BattleField::removeObject(BFObject *o)
{
    manager.removeObject(o);
}

BFManager *BattleField::getManager()
{
    return &manager;
}

void BattleField::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glBlendFunc(GL_ONE, GL_ONE);
    glDepthMask(GL_FALSE);
}

void BattleField::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    //qDebug("painting");
    manager.paintAll(this);
}

void BattleField::resizeGL(int w, int h)
{
    if(h == 0)
        h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(manager.getLeft(), manager.getRight(), manager.getBottom(), manager.getTop(), -1.0, 1.0);
    glLoadIdentity();
}

/*
void BattleField::paintEvent(QPaintEvent *paintevent)
{
    QPainter *painter = new QPainter(this);
    manager.paintAll(painter);
    delete painter;
}
*/

void BattleField::keyPressEvent(QKeyEvent *event)
{
    manager.keyPressEvent(event);
}

void BattleField::keyReleaseEvent(QKeyEvent *event)
{
    manager.keyReleaseEvent(event);
}

void BattleField::mouseMoveEvent(QMouseEvent *event)
{
    mouseEvent(event);
}

void BattleField::mousePressEvent(QMouseEvent *event)
{
    mouseEvent(event);
}

void BattleField::mouseReleaseEvent(QMouseEvent *event)
{
    mouseEvent(event);
}

void BattleField::refresh()
{
    manager.nextFrame(refresh_interval / 1000.0);
    display_counter++;
    if (display_counter == display_refreshes)
    {
        display_counter = 0;
        update();
    }
}

void BattleField::mouseEvent(QMouseEvent *mouseevent)
{
    double px = (double)mouseevent->x() / (double)width();
    double py = (double)mouseevent->y() / (double)height();
    manager.mouseEvent(Vector2d(px * manager.getRight() + (1.0 - px) * manager.getLeft(),
                                py * manager.getBottom() + (1.0 - py) * manager.getTop()),
                       mouseevent->buttons());
}
