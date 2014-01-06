#ifndef BATTLEFIELD_H
#define BATTLEFIELD_H

#include <QKeyEvent>
#include <QMouseEvent>
#include <QGLWidget>
#include <QTimer>
#include "BFManager.h"

class BattleField : public QGLWidget
{
    Q_OBJECT
public:
    static int refresh_interval;
    static int display_refreshes; //# of refreshes to re-display
    static double timescale; //scaling time. <1 slower, >1 faster

    explicit BattleField(QWidget *parent = 0);
    bool insertObject(BFObject *o); //unused, should be deleted later
    void removeObject(BFObject *o); //unused, should be deleted later
    BFManager *getManager();
    
signals:
    
public slots:
    void start();
    void pause();
    
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    //methods below are for BFCHuman
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

private slots:
    void refresh();

private:
    BFManager manager;
    QTimer refreshtimer;
    void mouseEvent(QMouseEvent *mouseevent);
    int display_counter;

};

#endif // BATTLEFIELD_H
