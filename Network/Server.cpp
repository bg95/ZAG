#include <QtWidgets>
#include <QtNetwork>
#include <QTimer>
#include <QList>

#include "BattleField/BFObject/BFOCircle.h"
#include "BattleField/BFObject/BFOColoredCircle.h"
#include "BattleField/BFController/BFCHuman.h"
#include "BattleField/BFController/BFCAIRandom.h"

#include "BattleField/BFRule/BFRCollision.h"
#include "BattleField/BFRule/BFRShoot.h"
#include "Network/Server.h"

#include "BattleField/BFFactory.h"

//#include "main.h"

Server::Server(QWidget *parent):
    QDialog(parent), tcpServer(0), networkSession(0), gameOn(false){

    // This part is for test
    debuggerLabel = new QLabel;
    statusLabel = new QLabel;
    sentMessage = new QLineEdit;
    //connect(sentMessage, SIGNAL(textChanged(QString)), this, SLOT(encodeMessage()));

    quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    gameBeginButton = new QPushButton(tr("GameBegin"));
    gameBeginButton->setAutoDefault(false);
    connect(gameBeginButton, SIGNAL(clicked()), this, SLOT(gameBegin()));
    sendMessageButton = new QPushButton(tr("SendMessage"));
    connect(sendMessageButton, SIGNAL(clicked()), this, SLOT(sendMessage()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(sendMessageButton);
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(gameBeginButton);

    participantList = new QListWidget;
    participantList->addItem(tr("Host"));
    nickNameList << QString("Host");

    messageList = new QListWidget;

    QVBoxLayout *partLayout = new QVBoxLayout;
    partLayout->addWidget(debuggerLabel);
    partLayout->addWidget(statusLabel);
    partLayout->addWidget(sentMessage);
    partLayout->addLayout(buttonLayout);
    partLayout->addWidget(messageList, 3);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(partLayout);
    mainLayout->addWidget(participantList);
    setLayout(mainLayout);

    setWindowTitle(tr("Server Test"));
    //Test part end */

    QNetworkConfigurationManager manager;
    if(manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired){
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if((config.state() & QNetworkConfiguration::Discovered) != QNetworkConfiguration::Discovered){
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, SIGNAL(opended()), this, SLOT(sessionOpened()));

        networkSession -> opened();
    }
    else{
        sessionOpened();
    }

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(acceptConnection()));

    /*
    networkTimer = new QTimer(this);
    connect(networkTimer, SIGNAL(timeout()), this, SLOT(checkMessage()));
    networkTimer->start(100);
    */
}

Server::~Server(){
    delete debuggerLabel;
    delete statusLabel;
    delete quitButton;
    delete sentMessage;
    delete participantList;
    delete messageList;
    /*if(clientConnection != NULL){
        clientConnection -> disconnectFromHost();
    }*/
}

void Server::acceptConnection(){
    debuggerLabel->setText(tr("Authentification"));

    if(gameOn){
        return;
    }

    QTcpSocket *newConnection = tcpServer->nextPendingConnection();
    QByteArray block = writeString("@Greeting");
    //QDataStream out(&block, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_4_0);
    //out << tr("@Greeting");
    newConnection->write(block);
    clientConnection = newConnection;
    QTimer::singleShot(1 * 1000, this, SLOT(auth()));
}

void Server::sendMessage(){
    //This part is for test
    debuggerLabel->setText(tr("Messages sent!"));
    //Test part end

    QString mes;
    mes += "#Host:: ";
    mes += sentMessage->text();
    foreach(QTcpSocket *connection, connectionList){
        connection->readAll();
        connection->write(writeString(mes));
    }
    messageList->addItem("Host:: " + sentMessage->text());
}

void Server::sessionOpened(){

    //tihs part is for test
    debuggerLabel -> setText(tr("Session is opened!"));
    //end test part

    if (networkSession) {
        QNetworkConfiguration config = networkSession->configuration();
        QString id;
        if (config.type() == QNetworkConfiguration::UserChoice)
            id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
        else
            id = config.identifier();

        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
        settings.endGroup();
    }

    tcpServer = new QTcpServer(this);
    if(!tcpServer->listen()){
        QMessageBox::critical(this, tr("Server"),
                              tr("Unable to be started: %1.")
                              .arg(tcpServer -> errorString()));
        close();
        return;
    }

    QString ipAddress;
    QList<QHostAddress> ipAddressList = QNetworkInterface::allAddresses();
    for(int i = 0; i != ipAddressList.size(); i++){
        if(ipAddressList.at(i) != QHostAddress::LocalHost &&
           ipAddressList.at(i).toIPv4Address()){
            ipAddress = ipAddressList.at(i).toString();
            break;
        }
    }

    if(ipAddress.isEmpty()){
        ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
    }

    //This part is for test
    statusLabel -> setText(tr("The server is running on \n\n IP: %1\nport: %2\n\n").arg(ipAddress).arg(tcpServer -> serverPort()));
}

/*
void Server::encodeMessage(){
    messages = sentMessage->text();
}
*/
/*
QByteArray *Server::getMessage(){

    QByteArray block = writeString(sentMessage->text());
    //QDataStream out(block, QIODevice::WriteOnly);
    //out.setVersion(QDataStream::Qt_4_0);

    //out << sentMessage->text();
    return block;
}
*/
void Server::auth(){
    qDebug("Verifying");
    debuggerLabel->setText(tr("Verifiring"));
    if(clientConnection->bytesAvailable() > 0){
        QDataStream in(clientConnection);
        in.setVersion(QDataStream::Qt_4_0);
        QString nickName;
        in >> nickName;

        connect(clientConnection, SIGNAL(readyRead()), this, SLOT(newMessage()));
        connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));
        connectionList << clientConnection;
        participantList->addItem(nickName);
        nickNameList << nickName;

        QString mes;
        foreach(QString nickName, nickNameList){
            mes += ("@" + nickName);
        }

        QByteArray block = writeString(mes);
        foreach(QTcpSocket *connection, connectionList){
            connection->readAll();
            connection->write(block);
        }
    }
    else{
        debuggerLabel->setText(tr("Rejected"));
        clientConnection->disconnectFromHost();
    }
}

QByteArray Server::writeString(QString str){
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << str;
    return block;
}

void Server::newMessage(){
    QString mesList;
    foreach(QTcpSocket *connection, connectionList){
        if(connection->bytesAvailable() > 0){
            QString mes;
            QDataStream in(connection);
            in.setVersion(QDataStream::Qt_4_0);
            in >> mes;
            messageList->addItem(mes);

            mesList += "#";
            mesList += mes;
        }
    }

    foreach(QTcpSocket *connection, connectionList){
        connection->write(writeString(mesList));
    }
}

//****************************************
//The following part is used for the game

void Server::gameBegin(){
    gameOn = true;
    gameBeginButton->setEnabled(false);

    QByteArray block = writeString("@GameBegin");
    foreach(QTcpSocket *connection, connectionList){
        connection->write(block);
    }

    bf = new BattleField(0);
    bfRule = new BFRShoot(bf->getManager());
    bf->getManager()->setRule(bfRule);
    connect(bf, SIGNAL(battleEnd()), this, SLOT(battleEnd()));
    connect(bf, SIGNAL(sendMessage(QByteArray)), this, SLOT(updateClient(QByteArray)));

    prepareInitialState();

    bf->start();
    this->hide();
    bf->show();

/*
    networkTimer = new QTimer(this);
    connect(networkTimer, SIGNAL(timeout()), this, SLOT(updateNetwork()));
    networkTimer->start(1000);
*/
}

void Server::prepareInitialState(){
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

    circle = new BFOColoredCircle;//(bf->getManager());
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
    bf->getManager()->insertObject(circle);

    BFCHuman *hum = new BFCHuman(bf->getManager(), circle);
    //BFCAIRandom *air = new BFCAIRandom(bf->getManager(), circle);
    bf->getManager()->registerController(hum);
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
*/
    hum = new BFCHuman(bf->getManager(), circle);
    bf->getManager()->registerController(hum);

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
    buf->open(QBuffer::WriteOnly);
    fac->encodeObject(circle, buf);
    buf->close();
    qDebug("prototype id = %ld", circle->getID());
    //delete circle;
    bf->getManager()->getFactory()->deleteObject(circle);

    buf->open(QBuffer::ReadOnly);
    for (int i = 0; i < 5; i++)
    {
        buf->seek(0);
        circle = (BFOColoredCircle *)fac->decodeNewObject(buf);
        circle->p = Vector2d(i / 8.0 - 0.8, 0.9);
        bf->getManager()->insertObject(circle);
        controller = new BFCAIRandom(bf->getManager(), circle);
        bf->getManager()->registerController(controller);
        //circles[i] = circle;
    }
    for (int i = 0; i < 5; i++)
    {
        buf->seek(0);
        circle = (BFOColoredCircle *)fac->decodeNewObject(buf);
        circle->p = Vector2d(-(i / 8.0 - 0.9), 0.9);
        bf->getManager()->insertObject(circle);
        controller = new BFCAIRandom(bf->getManager(), circle);
        bf->getManager()->registerController(controller);
        //circles[i] = circle;
    }
    buf->close();
    delete buf;
}

/*
void Server::updateNetwork(){
    foreach(QTcpSocket *connection, connectionList){
        sendMessage(connection);
    }
    //networkTimer->start(1000);
}
*/

void Server::battleEnd(){
    delete bf;
    delete bfRule;

    this->show();
}

void Server::updateClient(QByteArray message){
    foreach(QTcpSocket *client, connectionList){
        client->readAll();
        client->write(message);

//Temporary just for test
        //QByteArray block = writeString("This is a test message");
        //client->write(block);
    }
}
