#include <QtWidgets>
#include <QtNetwork>

#include "client.h"

Client::Client(QWidget *parent): QDialog(parent), networkSession(0){
    //This is for test
    hostLabel = new QLabel(tr("Server IP:"));
    portLabel = new QLabel(tr("Server port:"));

    hostEdit = new QComboBox;
    hostEdit -> setEditable(true);
    QString name = QHostInfo::localHostName();
    if(!name.isEmpty()){
        hostEdit -> addItem(name);
        QString domain = QHostInfo::localDomainName();
        if(!domain.isEmpty()){
            hostEdit -> addItem(name + QChar('.') + domain);
        }
    }
    if(name != QString("localhost")){
        hostEdit -> addItem(QString("localhost"));
    }
    QList<QHostAddress> ipAddressList = QNetworkInterface::allAddresses();
    for(int i = 0; i < ipAddressList.size(); i++){
        if(!ipAddressList.at(i).isLoopback()){
            hostEdit -> addItem((ipAddressList.at(i).toString()));
        }
    }
    for(int i = 0; i < ipAddressList.size(); i++){
        if(ipAddressList.at(i).isLoopback()){
            hostEdit -> addItem(ipAddressList.at(i).toString());
        }
    }
    portEdit = new QLineEdit;
    portEdit -> setValidator(new QIntValidator(1, 65535, this));

    portLabel -> setBuddy(portEdit);

    statusLabel = new QLabel(tr("This is for test."));

    getMessageButton = new QPushButton(tr("GetMessage"));
    getMessageButton -> setDefault(true);

    quitButton = new QPushButton(tr("Quit"));

    buttonBox = new QDialogButtonBox;
    buttonBox -> addButton(getMessageButton, QDialogButtonBox::ActionRole);
    buttonBox -> addButton(quitButton, QDialogButtonBox::RejectRole);

    connect(getMessageButton, SIGNAL(clicked()), this, SLOT(requestNewMessage()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(hostEdit, SIGNAL(editTextChanged(QString)), this, SLOT(setHostAndPort()));
    connect(portEdit, SIGNAL(textChanged(QString)), this, SLOT(setHostAndPort()));
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout -> addWidget(hostLabel, 0, 0);
    mainLayout -> addWidget(hostEdit, 0, 1);
    mainLayout -> addWidget(portLabel, 1, 0);
    mainLayout -> addWidget(portEdit, 1, 1);
    mainLayout -> addWidget(statusLabel, 2, 0, 1, 2);
    mainLayout -> addWidget(buttonBox, 3, 0, 1, 2);
    setLayout(mainLayout);

    setWindowTitle(tr("ZAG client"));

    //End test part */

    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));

    //Set hostname
    /*QString name = QHostInfo::localDomainName();
    if(!name.isEmpty()){
        hostName = name;
        QString domain = QHostInfo::localDomainName();
        if(!domain.isEmpty()){
            hostName = name + QChar('.') + domain;
        }
    }*/

    //QList<QHostAddress> ipAddressList = QNetworkInterface::allAddresses();

    QNetworkConfigurationManager manager;
    if(manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired){
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        //If the saved network configuration is not discovered, use the system's default configuration
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if((config.state() & QNetworkConfiguration::Discovered) != QNetworkConfiguration::Discovered){
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, SIGNAL(opened()), this, SLOT(sessionOpened()));

        networkSession -> open();

    }
}

void Client::setHostAndProt(){
    hostName = hostEdit -> currentText();
    port = (portEdit -> text()).toInt();
}

void Client::requestNewMessage(){
    blockSize = 0;
    tcpSocket -> abort();
    tcpSocket -> connectToHost(hostName, port);
}

void Client::readMessage(){
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    if(blockSize == 0){
        if(tcpSocket -> bytesAvailable() < (int)sizeof(quint16))
            return;

        in >> blockSize;
    }

    if(tcpSocket -> bytesAvailable() < blockSize)
        return;

    QString nextMessage;
    in >> nextMessage;

    if(nextMessage == currentMessage){
        QTimer::singleShot(0, this, SLOT(requestNewMessage()));
        return;
    }

    currentMessage = nextMessage;
}

void Client::displayError(QAbstractSocket::SocketError socketError){
    switch(socketError){
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Client"), tr("The host was not found!"));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Client"), tr("This connection is refused by the peer."));
        break;
    default:
        QMessageBox::information(this, tr("Client"), tr("The following error occurred: %1").arg(tcpSocket -> errorString()));
    }
}

void Client::sessionOpened(){
    QNetworkConfiguration config = networkSession -> configuration();
    QString id;
    if(config.type() == QNetworkConfiguration::UserChoice){
        id = networkSession -> sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    }
    else{
        id = config.identifier();
    }

    QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();
}
