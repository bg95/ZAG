#include <QtWidgets>
#include "BattleField/BFObject/BFOCircle.h"
#include "BattleField/BFObject/BFOColoredCircle.h"
#include "BattleField/BFController/BFCHuman.h"

#include "BattleField/BFRule/BFRCollision.h"
#include "Network/client.h"
#include "Network/server.h"

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "main.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    bf(0)
    //QMainWindow(parent)//,
    //ui(new Ui::MainWindow)
{
    //ui->setupUi(this);

    serverButton = new QPushButton(tr("Network server"));
    clientButton = new QPushButton(tr("Network client"));
    singleButton = new QPushButton(tr("Single player"));
    quitButton = new QPushButton(tr("Quit"));

    serverButton -> setAutoDefault(false);
    clientButton -> setAutoDefault(false);
    singleButton -> setAutoDefault(false);
    quitButton -> setAutoDefault(false);

    connect(serverButton, SIGNAL(clicked()), this, SLOT(server()));
    connect(clientButton, SIGNAL(clicked()), this, SLOT(client()));
    connect(singleButton, SIGNAL(clicked()), this, SLOT(singlePlayer()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout -> addWidget(singleButton);
    mainLayout -> addWidget(serverButton);
    mainLayout -> addWidget(clientButton);
    mainLayout -> addWidget(quitButton);

    setLayout(mainLayout);
    setWindowTitle(tr("ZAG"));
    //setCentralWidget(mainLayout);
}

void MainWindow::singlePlayer(){
    setGeometry(50, 50, 600, 600);

    bf = new BattleField(this);
    bf->setGeometry(0, 0, width(), height());
    bf->show();

    rule = new BFRCollision(bf->getManager());
    bf->getManager()->setRule(rule);

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
        circle = new BFOColoredCircle;//(bf->getManager());
        circle->p = Vector2d(i / 8.0 - 0.9, 0.9);
        circle->r = 0.05;
        circle->v = Vector2d(0.8, 0.5);
        circle->m = 0.25;
        bf->getManager()->insertObject(circle);
        circles[i] = circle;
    }
    circle = new BFOColoredCircle;//(bf->getManager());
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
        circle = new BFOColoredCircle;//(bf->getManager());
        circle->p = Vector2d(-(i / 8.0 - 0.9), 0.9);
        circle->r = 0.05;
        circle->v = Vector2d(-0.8, 0.5);
        circle->m = 0.25;
        bf->getManager()->insertObject(circle);
        circles[i] = circle;
    }

    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 50; j++)
        {
            circle = new BFOColoredCircle;//(bf->getManager());
            circle->p = Vector2d(i / 80.0 - 0.5, -0.9 + j / 80.0);
            circle->r = 0.005;
            circle->v = Vector2d(i / 350.0 - 0.5, 0.5 + i / 180.0 - 0.5);
            circle->m = 0.0025;
            bf->getManager()->insertObject(circle);
        }
    bf -> update();

}

void MainWindow::server(){
    Server *server = new Server;
    server -> show();
    return;
}

void MainWindow::client(){
    Client *client = new Client;
    client -> show();
    return;
}

MainWindow::~MainWindow()
{
    delete serverButton;
    delete clientButton;
    delete singleButton;
    delete quitButton;
    //delete ui;
    //delete bf;
    //delete rule;
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    if (bf)
        bf->setGeometry(0, 0, width(), height());
}
