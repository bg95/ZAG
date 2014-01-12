#include <QtWidgets>
#include "BattleField/BFObject/BFOCircle.h"
#include "BattleField/BFObject/BFOColoredCircle.h"
#include "BattleField/BFController/BFCHuman.h"
#include "BattleField/BFController/BFCAIRandom.h"
#include "BattleField/BFController/BFCRandomShootDodge.h"

#include "BattleField/BFRule/BFRCollision.h"
#include "Network/Client.h"
#include "Network/Server.h"
#include "BattleField/BFRule/BFRShoot.h"

#include "BattleField/BFFactory.h"

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <sstream>
#include <QBuffer>

#include "main.h"

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    bf(0),
    server(0),
    client(0)
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

    connect(serverButton, SIGNAL(clicked()), this, SLOT(serverMode()));
    connect(clientButton, SIGNAL(clicked()), this, SLOT(clientMode()));
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
    /*setGeometry(50, 50, 600, 600);

    bf = new BattleField(this);
    bf->setGeometry(0, 0, width(), height());
    bf->show();*/
    //setGeometry(mainWindowLUx, mainWindowLUy, mainWindowWidth, mainWindowHeight);

    bf = new BattleField(0);
    //bf->setGeometry(0, 0, width(), height());

    //rule = new BFRCollision(bf->getManager());
    rule = new BFRShoot(bf->getManager());
    bf->getManager()->setRule(rule);
    connect(bf, SIGNAL(battleEnd()), this, SLOT(battleEnd()));

    BFOColoredCircle *circle;
    BFCAIRandom *controller;

    BFOColoredCircle *bullet = (BFOColoredCircle *)bf->getManager()->getFactory()->newObject(typehash(BFOColoredCircle));
    bullet->setColor(1.0, 0, 0, 1.0);
    bullet->r = 0.01;
    bullet->v = Vector2d(0, 6);
    bullet->m = 0.01;
    bullet->setProperty("isBullet", "Yes");
    bullet->setProperty("damage", 0.2);
    QBuffer bulletbuf;
    bulletbuf.open(QIODevice::ReadWrite);
    bf->getManager()->getFactory()->encodeObject(bullet, &bulletbuf);
    bulletbuf.seek(0);
    bf->getManager()->getFactory()->deleteObject(bullet);
    //shooter property is set in BFRShoot

    //circle = new BFOColoredCircle;//(bf->getManager());
    circle = (BFOColoredCircle *)bf->getManager()->getFactory()->newObject(typehash(BFOColoredCircle));
    //bullet->setProperty("shooter", (unsigned long long)circle);
    circle->p = Vector2d(0, 0.9);
    circle->r = 0.1;
    circle->v = Vector2d(0, 0.5);
    circle->m = 1;
    circle->maxa = 5;
    circle->setColor(0.0, 0.5, 1.0, 1.0);
    circle->setProperty("shoot", "");
    circle->setProperty("bullet prototype", bulletbuf.data());
    circle->setProperty("cooldown", 0.05);
    circle->setProperty("cooldowncount", 0.0);
    circle->setProperty("health", 1.0);
    circle->setProperty("fraction", 0);
    bf->getManager()->insertObject(circle);

    BFCHuman *ctrl = new BFCHuman(bf->getManager(), circle->getID());
    //BFCRandomShootDodge *ctrl = new BFCRandomShootDodge(bf->getManager(), circle);
    //BFCAIRandom *ctrl = new BFCAIRandom(bf->getManager(), circle);
    bf->getManager()->registerController(ctrl);
/*
    circle = new BFOColoredCircle;//(bf->getManager());
    //bullet->setProperty("shooter", (unsigned long long)circle);
    circle->p = Vector2d(0, 0.8);
    circle->r = 0.05;
    circle->v = Vector2d(0, 0.5);
    circle->m = 0.25;
    circle->maxa = 5;
    circle->setColor(0.0, 0.5, 1.0, 1.0);
    circle->setProperty("shoot", "");
    circle->setProperty("bullet prototype", bulletbuf.data());
    circle->setProperty("cooldown", 0.1);
    circle->setProperty("cooldowncount", 0.0);
    circle->setProperty("health", 1.0);
    bf->getManager()->insertObject(circle);

    hum = new BFCHuman(bf->getManager(), circle);
    bf->getManager()->registerController(hum);
*/
    QBuffer *buf = new QBuffer;
    BFFactory *fac = bf->getManager()->getFactory();
    circle = (BFOColoredCircle *)bf->getManager()->getFactory()->newObject(typehash(BFOColoredCircle));//(bf->getManager());
    circle->p = Vector2d(-0.7, 0.9);
    circle->r = 0.05;
    circle->v = Vector2d(0.8, 0.5);
    circle->m = 0.25;
    circle->maxa = 5;
    circle->setProperty("shoot", "");
    circle->setProperty("health", 1.0);
    //(*circle)["fraction"] = 2;
    circle->setProperty("fraction", 2);
    buf->open(QBuffer::WriteOnly);
    fac->encodeObject(circle, buf);
    buf->close();
    qDebug("prototype id = %ld", circle->getID());
    //delete circle;
    bf->getManager()->getFactory()->deleteObject(circle);

    buf->open(QBuffer::ReadOnly);
    /*for (int i = 0; i < 5; i++)
    {
        buf->seek(0);
        circle = (BFOColoredCircle *)fac->decodeNewObject(buf);
        circle->p = Vector2d(i / 8.0 - 0.8, 0.9);
        bf->getManager()->insertObject(circle);
        controller = new BFCAIRandom(bf->getManager(), circle->getID());
        bf->getManager()->registerController(controller);
        //circles[i] = circle;
    }
    for (int i = 0; i < 5; i++)
    {
        buf->seek(0);
        circle = (BFOColoredCircle *)fac->decodeNewObject(buf);
        circle->p = Vector2d(-(i / 8.0 - 0.9), 0.9);
        bf->getManager()->insertObject(circle);
        controller = new BFCAIRandom(bf->getManager(), circle->getID());
        bf->getManager()->registerController(controller);
        //circles[i] = circle;
    }*/
    buf->close();
    delete buf;
/*
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 5; j++)
        {
            circle = new BFOColoredCircle;//(bf->getManager());
            circle->p = Vector2d(i / 80.0 - 0.5, -0.9 + j / 80.0);
            circle->r = 0.005;
            circle->v = Vector2d(i / 350.0 - 0.5, 0.5 + i / 180.0 - 0.5);
            circle->m = 0.0025;
            circle->setProperty("shoot", "");
            circle->setProperty("health", 0.4);
            bf->getManager()->insertObject(circle);
        }
    bf -> update();
*/

    bf->show();
    this->hide();
    bf->start();
}

void MainWindow::serverMode(){
    server = new Server;
    server -> show();
    return;
}

void MainWindow::clientMode(){
    client = new Client;
    client -> show();
    return;
}

MainWindow::~MainWindow()
{
    delete serverButton;
    delete clientButton;
    delete singleButton;
    delete quitButton;
    if(server != NULL){
        delete server;
    }
    if(client != NULL){
        delete client;
    }
    //delete ui;
    //delete bf;
    //delete rule;
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    if (bf)
        bf->setGeometry(0, 0, width(), height());
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    qDebug("mainwindow key %d", event->key());
}

void MainWindow::battleEnd(){
    delete bf;
    delete rule;
    this->show();
}
