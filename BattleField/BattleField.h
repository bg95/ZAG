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

    explicit BattleField(QWidget *parent = 0);
    bool insertObject(BFObject *o);
    void removeObject(BFObject *o);
    BFManager *getManager();
    
signals:
    
public slots:
    
protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

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

};

#endif // BATTLEFIELD_H
