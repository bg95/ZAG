#include <QPainter>
#include <QBuffer>
#include <math.h>
#include <cmath>
#include <GL/gl.h>
#include "Geometry.h"
#include "../main.h"
#include "BattleField.h"

int BattleField::refresh_interval = 5;
int BattleField::display_refreshes = 6;
double BattleField::timescale = 1.0;

BattleField::BattleField(QWidget *parent, bool fs) :
    main_window(parent),
    unit(1.0),
    angle(0.0),
    delta_x(0.0),
    delta_y(0.0),
    QGLWidget(parent),
    refreshtimer(this),
    display_counter(0),
    manager(this)//,
    //overlay(this)
{
    refreshtimer.setInterval(refresh_interval);
    connect(&refreshtimer, SIGNAL(timeout()), this, SLOT(refresh()));
    //connect(&refreshtimer, SIGNAL(timeout()), &overlay, SLOT(update()));
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
                                   { 0.0, 0.0, -0.5, 1.0 }};
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

    glPushMatrix();
        glTranslatef(delta_x, delta_y, 0.0);
        glRotatef(angle, 0.0, 0.0, 1.0);
        glScalef(unit, unit, unit);
        drawGrid();
        manager.paintAll(this);
    glPopMatrix();
    //qDebug("painting");
}

void BattleField::drawGrid() {
    int numBar = 35;
    double step = 2.0 / (double)numBar;
    double radius = step / 20;
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glPushMatrix();
//        glTranslatef(delta_x, delta_y, 0.0);
        glPushMatrix();
            glRotatef(-90.0, 1.0, 0.0, 0.0);
            glTranslatef(-1.0, 0.0, -1.0);
            for(double x = 0; x <= 2.0 + step; x += step) {
                for(double z = 0; z < 2.0; z += step) {
                    glutSolidCylinder(radius, step, 6, 10);
                    glTranslated(0.0, 0.0, step);
                }
                glTranslated(0.0, 0.0, -2.0);
                glTranslated(step, 0.0, 0.0);
            }
        glPopMatrix();
        glPushMatrix();
            glRotatef(90.0, 0.0, 1.0, 0.0);
            glTranslatef(0.0, -1.0, -1.0);
            for(double y = 0; y <= 2.0 + step; y += step) {
                glutSolidCylinder(radius, 2.0, 6, 10);
                glTranslated(0.0, step, 0.0);
            }
        glPopMatrix();
    glPopMatrix();
}

void BattleField::resizeGL(int w, int h)
{
    if(h == 0)
        h = 1;

    int size = w < h ? w : h;
    double scaleW = (double)w / (double) size;
    double scaleH = (double)h / (double) size;
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-0.65 * scaleW, 0.65 * scaleW, -0.65 * scaleH, 0.65 * scaleH, 1.0, 3.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -2.0);
    glRotatef(-15.0, 1.0, 0.0, 0.0);

    //overlay.setGeometry(0, 0, width() / 4, height() / 4);
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
    case Qt::Key_Up:
        move(0.0, -0.3);
        return;
    case Qt::Key_Down:
        move(0.0, 0.3);
        return;
    case Qt::Key_Left:
        move(0.3, 0.0);
        return;
    case Qt::Key_Right:
        move(-0.3, 0.0);
        return;
    case Qt::Key_J://rotate counter-clockwise
        rotate(10.0);
        return;
    case Qt::Key_K://rotate clockwise
        rotate(-10.0);
        return;
    case Qt::Key_H://zoom in, this can be raplaced by wheel of mouse
        scale(1.2);
        return;
    case Qt::Key_L://zoom out
        scale(0.83333);
        return;
    case Qt::Key_Escape://escape
        //this->main_window->close();
        this->close();
        emit battleEnd();
        return;
    }
    manager.keyPressEvent(event);
}

void BattleField::scale(double k)
{
    if(unit * k < 20 && unit * k > 0.8)
    {
        delta_x *= k;
        delta_y *= k;
        unit *= k;
    }
}

double BattleField::getScale() const
{
    return unit;
}

void BattleField::move(double dx, double dy)
{
    if(pow(delta_x + dx, 2.0) + pow(delta_y + dy, 2.0) >= pow(unit, 2.0) * 2.0)
        return;
    delta_x += dx;
    delta_y += dy;
}

Vector2d BattleField::getTranslation() const
{
    return Vector2d(delta_x, delta_y);
}

void BattleField::rotate(double dangle)
{
    angle += dangle;
    double tmp_x = delta_x, tmp_y = delta_y;
    dangle = dangle * PI / 180.0;
    delta_x = tmp_x * cos(dangle) - tmp_y * sin(dangle);
    delta_y = tmp_x * sin(dangle) + tmp_y * cos(dangle);
    qDebug("After rotation, angle = %f, delta_x = %f, delta_y = %f", angle, delta_x, delta_y);
}

double BattleField::getRotation() const
{
    return angle / 180.0 * PI;
}

void BattleField::wheelEvent(QWheelEvent *event)
{
    double delta = event->delta() / ( 8.0 * 15.0) / 10.0;
    scale(exp(delta));
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
    manager.nextFrame(refresh_interval / 1000.0 * timescale);

//Send message to clients
    //QByteArray message;
    //QBuffer buf(&message);
    //manager.encodeAllObjects(&buf);

    //Test message
    QByteArray message;
    QDataStream out(&message, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << "This is a message";
    //End

    emit sendMessage(message);

    display_counter++;
    if (display_counter == display_refreshes)
    {
        display_counter = 0;
        update();
    }
}

void BattleField::mouseEvent(QMouseEvent *mouseevent)
{
/*    double px = (double)mouseevent->x() / (double)width();
    double py = (double)mouseevent->y() / (double)height();

    manager.mouseEvent(Vector2d(px * manager.getRight() + (1.0 - px) * manager.getLeft(),
                                py * manager.getBottom() + (1.0 - py) * manager.getTop()),
                       mouseevent->buttons());*/
    double w = (double)width();
    double h = (double)height();
    double size = w < h ? w : h;
    double x_prime = ( (double)mouseevent->x() - w / 2 ) / (w / 2.0) * (w / size * 0.65);
    double y_prime = - ( (double)mouseevent->y() - h / 2 ) / (h / 2.0) * ( h / size * 0.65);
    double tan15 = tan(15.0 * PI / 180.0), cos15 = cos(15.0 * PI / 180.0);
    double z = 2 / (y_prime * tan15 - 1);
    //x and y are the coordinates in the battlefield
    double y = - z * y_prime / cos15;//-1.0 ~ 1.0
    double x = - z * x_prime;//-1.0 ~ 1.0
    qDebug("x = %f, y = %f", x, y);
    double alpha = - ( angle * PI / 180.0 - atan2(y - delta_y, x - delta_x) );//in radian
    double r = sqrt(pow(x - delta_x, 2.0) + pow(y - delta_y, 2.0)) / unit;
    double X = r * cos(alpha), Y = r * sin(alpha);//-1.0 ~ 1.0
    qDebug("alpha = %f, X = %f, Y = %f", alpha * 180.0 / PI, X, Y);
    double px = (X + 1.0) / 2.0;
    double py = (Y + 1.0) / 2.0;
    qDebug("px = %f, py = %f\n", px, py);
    manager.mouseEvent(Vector2d(px * manager.getRight() + (1.0 - px) * manager.getLeft(),
                                (1.0 - py) * manager.getBottom() + py * manager.getTop()),
                       mouseevent->buttons());
}
