#include <QtWidgets>
#include "BattleField/BFObject/BFOCircle.h"
#include "BattleField/BFObject/BFOColoredCircle.h"
#include "BattleField/BFController/BFCHuman.h"
#include "BattleField/BFController/BFCAIRandom.h"
#include "BattleField/BFController/BFCRandomShootDodge.h"
#include "BattleField/BFController/BFCHumanAndRSD.h"

#include "BattleField/BFRule/BFRCollision.h"
#include "Network/Client.h"
#include "Network/Server.h"
#include "BattleField/BFRule/BFRShoot.h"
#include "BattleField/BFRule/BFRSurvival.h"

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
    rule = new BFRSurvival(bf->getManager());
    bf->getManager()->setRule(rule);
    connect(bf, SIGNAL(battleEnd()), this, SLOT(battleEnd()));

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
