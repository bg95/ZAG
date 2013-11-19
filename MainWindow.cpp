#include "BattleField/BFObject/BFOCircle.h"
#include "BattleField/BFObject/BFOColoredCircle.h"
#include "BattleField/BFController/BFCHuman.h"

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "main.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)//,
    //ui(new Ui::MainWindow)
{
    //ui->setupUi(this);
    setGeometry(50, 50, 600, 600);

    bf = new BattleField(this);
    bf->setGeometry(0, 0, width(), height());

    BFOColoredCircle *circle;
/*
    circle = new BFOColoredCircle(bf->getManager());
    circle->p = Vector2d(0.89, 0.05);
    circle->r = 0.1;
    circle->v = Vector2d(-0.4, 0);
    circle->m = 1;
    circle->maxa = 5;
    bf->getManager()->insertObject(circle);
    BFCHuman *hum = new BFCHuman(circle);
    bf->getManager()->registerController(hum);*/
/*
    circle = new BFOColoredCircle(bf->getManager());
    circle->p = Vector2d(0, -0.05);
    circle->r = 0.1;
    circle->v = Vector2d(0.4, 0);
    circle->m = 1;
    bf->getManager()->insertObject(circle);

    circle = new BFOColoredCircle(bf->getManager());
    circle->p = Vector2d(-0.89, 0.03);
    circle->r = 0.14;
    circle->v = Vector2d(0.4, 0.1);
    circle->m = 2;
    bf->getManager()->insertObject(circle);
*//*
    circle = new BFOColoredCircle(bf->getManager());
    circle->p = Vector2d(-0.89, 0.03);
    circle->r = 0.05;
    circle->v = Vector2d(0.8, 0.5);
    circle->m = 5;
    bf->getManager()->insertObject(circle);
*/

    for (int i = 0; i < 5; i++)
    {
        circle = new BFOColoredCircle(bf->getManager());
        circle->p = Vector2d(i / 8.0 - 0.9, 0.9);
        circle->r = 0.05;
        circle->v = Vector2d(0.8, 0.5);
        circle->m = 0.25;
        bf->getManager()->insertObject(circle);
        circles[i] = circle;
    }
    circle = new BFOColoredCircle(bf->getManager());
    circle->p = Vector2d(0, 0.9);
    circle->r = 0.05;
    circle->v = Vector2d(0, 0.5);
    circle->m = 0.25;
    bf->getManager()->insertObject(circle);

    circle->maxa = 5;
    BFCHuman *hum = new BFCHuman(circle);
    bf->getManager()->registerController(hum);

    for (int i = 0; i < 5; i++)
    {
        circle = new BFOColoredCircle(bf->getManager());
        circle->p = Vector2d(-(i / 8.0 - 0.9), 0.9);
        circle->r = 0.05;
        circle->v = Vector2d(-0.8, 0.5);
        circle->m = 0.25;
        bf->getManager()->insertObject(circle);
        circles[i] = circle;
    }

    for (int i = 0; i < 50; i++)
        for (int j = 0; j < 20; j++)
        {
            circle = new BFOColoredCircle(bf->getManager());
            circle->p = Vector2d(i / 80.0 - 0.5, -0.9 + j / 80.0);
            circle->r = 0.005;
            //circle->v = Vector2d(i / 35.0 - 0.5, 0.5 + i / 18.0 - 0.5);
            circle->m = 0.0025;
            bf->getManager()->insertObject(circle);
        }

    bf->update();
}

MainWindow::~MainWindow()
{
    //delete ui;
    delete bf;
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    bf->setGeometry(0, 0, width(), height());
}
