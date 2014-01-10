#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

class MainWindow;
class BFManager;

#include <QKeyEvent>
#include <QMouseEvent>
#include <QGLWidget>
#include <QTimer>
#include "MainWindow.h"
#include "BFManager.h"
#include "BattleFieldOverlay.h"

class BattleField : public QGLWidget
{
    Q_OBJECT
public:
    static int refresh_interval;
    static int display_refreshes; //# of refreshes to re-display
    static double timescale; //scaling time. <1 slower, >1 faster

    explicit BattleField(QWidget *parent = 0, bool fs = false);
    bool insertObject(BFObject *o); //unused, should be deleted later
    void removeObject(BFObject *o); //unused, should be deleted later
    BFManager *getManager();

    //methods for changing the view port
    void scale(double k);
    double getScale() const;
    void move(double dx, double dy);
    Vector2d getTranslation() const;
    void rotate(double dangle); //in degree, rotate stands for positive degree
    double getRotation() const;



signals:
    
public slots:
    void start();
    void pause();
    
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);
    void gluPerspective(GLdouble, GLdouble, GLdouble, GLdouble);
    void drawGrid();

    //methods below are for BFCHuman
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);

private slots:
    void refresh();

private:
    QWidget* main_window;
    BFManager manager;
    QTimer refreshtimer;
    void mouseEvent(QMouseEvent *mouseevent);
    int display_counter;
    bool fullscreen;
    //information on the view port
    float unit;
    float angle;
    float delta_x, delta_y;

    BattleFieldOverlay overlay;

};

#endif // BATTLEFIELD_H
