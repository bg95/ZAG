#include <QtWidgets>
#include <QtNetwork>
#include <QTimer>
#include <QList>

#include "Server.h"

Server::Server(QWidget *parent):
    QDialog(parent), tcpServer(0), networkSession(0){

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

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    //buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);
    //buttonLayout->addStretch(1);
    buttonLayout->addWidget(gameBeginButton);

    participantList = new QListWidget;
    participantList->addItem(tr("Local Host"));

    QVBoxLayout *partLayout = new QVBoxLayout;
    partLayout->addWidget(debuggerLabel);
    partLayout->addWidget(statusLabel);
    partLayout->addWidget(sentMessage);
    partLayout->addLayout(buttonLayout);

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

    //networkTimer = new QTimer(this);
}

Server::~Server(){
    delete debuggerLabel;
    delete statusLabel;
    delete quitButton;
    delete sentMessage;
    delete participantList;
    /*if(clientConnection != NULL){
        clientConnection -> disconnectFromHost();
    }*/
}

void Server::acceptConnection(){
    debuggerLabel->setText(tr("Authentification"));
    QTcpSocket *newConnection = tcpServer->nextPendingConnection();
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << tr("@Greeting");
    newConnection->write(block);
    clientConnection = newConnection;
    QTimer::singleShot(1 * 1000, this, SLOT(auth()));
}

void Server::gameBegin(){
    networkTimer = new QTimer(this);
    connect(networkTimer, SIGNAL(timeout()), this, SLOT(updateNetwork()));
    networkTimer->start(1000);
}

void Server::updateNetwork(){
    foreach(QTcpSocket *connection, connectionList){
        sendMessage(connection);
    }
    networkTimer->start(1000);
}

void Server::sendMessage(QTcpSocket *connection){
    //This part is for test
    debuggerLabel->setText(tr("Messages sent!"));
    //Test part end

    QDataStream in(connection);
    in.setVersion(QDataStream::Qt_4_0);

    in >> currentMessageGot;
    debuggerLabel->setText(currentMessageGot);

    connection->write(*getMessage());
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
QByteArray *Server::getMessage(){

    QByteArray *block = new QByteArray;
    QDataStream out(block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << sentMessage->text();
    return block;
}

void Server::auth(){
    qDebug("Verifying");
    debuggerLabel->setText(tr("Verifiring"));
    if(clientConnection->bytesAvailable() > 0){
        QDataStream in(clientConnection);
        in.setVersion(QDataStream::Qt_4_0);
        QString nickName;
        in >> nickName;

        //connect(clientConnection, SIGNAL(readyRead()), this, SLOT(sendMessage()));
        connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));
        connectionList << clientConnection;
        participantList->addItem(nickName);
    }
    else{
        debuggerLabel->setText(tr("Rejected"));
        clientConnection->disconnectFromHost();
    }
}
