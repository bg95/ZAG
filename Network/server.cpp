#include <QtWidgets>
#include <QtNetwork>

#include "server.h"

Server::Server(QWidget *parent):
    QDialog(parent), tcpServer(0), networkSession(0){

    // This part is for test
    debuggerLabel = new QLabel;
    statusLabel = new QLabel;
    quitButton = new QPushButton(tr("Quit"));
    quitButton -> setAutoDefault(false);
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout -> addStretch(1);
    buttonLayout -> addWidget(quitButton);
    buttonLayout -> addStretch(1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout -> addWidget(debuggerLabel);
    mainLayout -> addWidget(statusLabel);
    mainLayout -> addLayout(buttonLayout);
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

    // Set transformed message
    // May be set another class later
    messages = encodeMessage();

    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(sendMessage()));
}


void Server::sendMessage(){
    //This part is for test
    debuggerLabel -> setText(tr("Messages sent!"));
    //Test part end

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);

    out << (quint16)0;
    //for(int i = 0; i != messages.size(); i++){
    //    out << messages.at(i);
    //}
    out << messages;
    out.device() -> seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    QTcpSocket *clientConnection = tcpServer -> nextPendingConnection();
    connect(clientConnection, SIGNAL(disconnected()),
            clientConnection, SLOT(deleteLater()));
    clientConnection -> write(block);
    clientConnection -> disconnectFromHost();
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


QString Server::encodeMessage(){
    QString newString;
    newString = QString("TestString");
    return newString;
}
