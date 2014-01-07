#include <QPainter>
#include <math.h>
#include <GL/gl.h>
#include "Geometry.h"
#include "../main.h"
#include "BattleField.h"

int BattleField::refresh_interval = 10;
int BattleField::display_refreshes = 1;

BattleField::BattleField(QWidget *parent, bool fs) :
    main_window(parent),
    zNear(10.0),
    delta_x(0.0),
    delta_y(0.0),
    QGLWidget(parent),
    refreshtimer(this),
    display_counter(0)
{
    refreshtimer.setInterval(refresh_interval);
    connect(&refreshtimer, SIGNAL(timeout()), this, SLOT(refresh()));
    //refreshtimer.start();
    fullscreen = fs;
    if(fullscreen)
        showFullScreen();
    setMouseTracking(true);
    grabKeyboard();
    grabMouse();
}

bool BattleField::insertObject(BFObject *o) //unused, should be deleted later
{
    return manager.insertObject(o);
}

void BattleField::removeObject(BFObject *o) //unused, should be deleted later
{
    manager.removeObject(o);
}

BFManager *BattleField::getManager()
{
    return &manager;
}

void BattleField::start()
{
    refreshtimer.start(refresh_interval);
}

void BattleField::pause()
{
    refreshtimer.stop();
}

void BattleField::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_BLEND);
    //set depth buffer
    glClearDepth(1.0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_LIGHTING);
    GLfloat lightAmbient[4][4] = {{ 0.0, 0.0, 0.0, 1.0 },
                                  { 0.0, 0.0, 0.0, 1.0 },
                                  { 0.0, 0.0, 0.0, 1.0 },
                                  { 0.0, 0.0, 0.0, 1.0 }};
    GLfloat lightDiffuse[4][4] = {{ 1.0, 0.0, 0.0, 1.0 },
                                  { 0.0, 1.0, 0.0, 1.0 },
                                  { 0.0, 0.0, 1.0, 1.0 },
                                  { 1.0, 1.0, 1.0, 1.0 }};
    GLfloat lightPosition[4][4] = {{ 15.0, 15.0, -18.0, 1.0 },
                                   { 15.0, -15.0, -18.0, 1.0 },
                                   { -15.0, -15.0, -18.0, 1.0 },
                                   { 0.0, 0.0, -9.0, 1.0 }};
    glLightfv( GL_LIGHT0, GL_AMBIENT, lightAmbient[0] );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, lightDiffuse[0] );
    glLightfv( GL_LIGHT0, GL_POSITION, lightPosition[0] );
    //glEnable( GL_LIGHT0 );
    glLightfv( GL_LIGHT1, GL_AMBIENT, lightAmbient[1] );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse[1] );
    glLightfv( GL_LIGHT1, GL_POSITION, lightPosition[1] );
    //glEnable( GL_LIGHT1 );
    glLightfv( GL_LIGHT2, GL_AMBIENT, lightAmbient[2] );
    glLightfv( GL_LIGHT2, GL_DIFFUSE, lightDiffuse[2] );
    glLightfv( GL_LIGHT2, GL_POSITION, lightPosition[2] );
    //glEnable( GL_LIGHT2 );
    glLightfv( GL_LIGHT3, GL_AMBIENT, lightAmbient[3] );
    glLightfv( GL_LIGHT3, GL_DIFFUSE, lightDiffuse[3] );
    glLightfv( GL_LIGHT3, GL_POSITION, lightPosition[3] );
    glEnable( GL_LIGHT3 );

    //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc(GL_ONE, GL_ONE);
    //glDepthMask(GL_FALSE);
}

void BattleField::paintGL()
{
    //clear the screen and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    //below is just for test
    glTranslatef(0.0, 10.0, -20.0);
    glRotatef(-15.0, 1.0, 0.0, 0.0);
    glTranslatef(0.0, -10.0, 0.0);

    drawBattleField();
    //qDebug("painting");
    manager.paintAll(this);
}

void BattleField::resizeGL(int w, int h)
{
    if(h == 0)
        h = 1;

    int size = w < h ? w : h;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glRotatef(-angle, 0.0, 0.0, 1.0);
    glFrustum(-7.5 * (double)w / (double)size + delta_x, 7.5 * (double)w / (double)size + delta_x,
              -7.5 * (double)h / (double)size + delta_y, 7.5 * (double)h / (double)size + delta_y,
              zNear, 20.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//This function has been replaced by glFrustum()=_+
void BattleField::gluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    GLdouble xmin, xmax, ymin, ymax;

    ymax = zNear * tan( fovy * PI / 360.0 );
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    glFrustum( xmin, xmax, ymin, ymax, zNear, zFar );
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
    switch(event->key()) {
    case Qt::Key_F1://full screen
        fullscreen = !fullscreen;
        if(fullscreen)  this->main_window->showFullScreen();
        else {
            this->main_window->showNormal();
            this->main_window->setGeometry(mainWindowLUx, mainWindowLUy, mainWindowWidth, mainWindowHeight);
        }
        updateGL();
        return;
    case Qt::Key_N:
        rotateClockwise(30.0);
        return;
    case Qt::Key_H:
        moveHorizontal(-1.0);
        return;
    case Qt::Key_J:
        moveVertical(-1.0);
        return;
    case Qt::Key_Escape://escape
        this->main_window->close();
        return;
    }
    manager.keyPressEvent(event);
}

void BattleField::zoom(double dz)
{
    if(zNear + dz <= 14.0 && zNear + dz >= 9.0)
    {
        qDebug("scale %lf\n", dz);
        zNear += dz;
        resizeGL(width(), height());
    }
}

void BattleField::moveHorizontal(double dx)
{

    delta_x += dx;
    resizeGL(width(), height());
}

void BattleField::moveVertical(double dy)
{
    delta_y += dy;
    resizeGL(width(), height());
}

void BattleField::rotateClockwise(double dangle)
{
    angle += dangle;
    resizeGL(width(), height());
}

void BattleField::wheelEvent(QWheelEvent *event)
{
    double delta_z = (double)event->delta() / 8.0 / 15.0;//.../8.0 is in degree
    zoom(delta_z);
    event->ignore();
    manager.wheelEvent(event);
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
