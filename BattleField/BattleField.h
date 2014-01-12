#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

class MainWindow;
class BFManager;
class Scene;

#include <QKeyEvent>
#include <QMouseEvent>
#include <QGLWidget>
#include <QTimer>
#include "MainWindow.h"
#include "BFManager.h"
#include "Scene.h"
//#include "BattleFieldOverlay.h"

//The widget to show the battle field
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

    //setting the object to be centered on the screen
    void setFocusObject(BFObjectID objid);
    void resetFocus();


signals:
    void timerTimeout();
    void battleEnd();
    void sendMessage(QByteArray);
    
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

    void refreshMousePosition();

private slots:
    void refresh();

private:
    QWidget* main_window;
    BFManager manager;
    Scene scene;
    QTimer refreshtimer;
    void mouseEvent(QMouseEvent *mouseevent);
    int display_counter;
    bool fullscreen;
    //information on the view port
    float unit;
    float angle;
    float delta_x, delta_y;

    int mouse_x, mouse_y;
    Qt::MouseButtons mouse_buttons;

    BFObjectID focusobj;
    bool setfocus;

    //BattleFieldOverlay overlay;
};

#endif // BATTLEFIELD_H
