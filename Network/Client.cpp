#include <QtWidgets>
#include <QtNetwork>

#include "BattleField/BFObject/BFOCircle.h"
#include "BattleField/BFObject/BFOColoredCircle.h"
#include "BattleField/BFController/BFCHuman.h"
#include "BattleField/BFController/BFCAIRandom.h"

#include "BattleField/BFRule/BFRCollision.h"
#include "BattleField/BFRule/BFRShoot.h"
#include "BattleField/BFController/BFCHumanAndRSD.h"
#include "Network/Client.h"

#include "BattleField/BFFactory.h"

/*
 *Client will prepare the UI to show and prepare the essentials for network
 *Client is inherited from QDialog, which will allow widgets of QT.
 *A list of local IP address is listed for the user, but the user should type himself if he want a internet game
 *
 */
Client::Client(QWidget *parent): QDialog(parent), networkSession(0), counter(0), blockSize(0){
    //This is UI part
    hostLabel = new QLabel(tr("Server IP:"));
    portLabel = new QLabel(tr("Server port:"));
    nickLabel = new QLabel(tr("Input your name:"));
    debuggerLabel = new QLabel(tr("This is for debug"));

    messageEdit = new QLineEdit;

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

    sendMessageButton = new QPushButton(tr("SendMessage"));
    //sendMessageButton -> setDefault(false);
    sendMessageButton->setEnabled(false);

    quitButton = new QPushButton(tr("Quit"));

    connectToHostButton = new QPushButton(tr("ConnectToHost"));

    playerList = new QListWidget;
    messageList = new QListWidget;

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(connectToHostButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(sendMessageButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    connect(connectToHostButton, SIGNAL(clicked()), this, SLOT(connectToHost()));
    connect(sendMessageButton, SIGNAL(clicked()), this, SLOT(sendMessage()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(hostEdit, SIGNAL(editTextChanged(QString)), this, SLOT(setHostAndPort()));
    connect(portEdit, SIGNAL(textChanged(QString)), this, SLOT(setHostAndPort()));

    QGridLayout *partLayout = new QGridLayout;
    partLayout->addWidget(hostLabel, 0, 0);
    partLayout->addWidget(hostEdit, 0, 1);
    partLayout->addWidget(portLabel, 1, 0);
    partLayout->addWidget(portEdit, 1, 1);
    partLayout->addWidget(statusLabel, 3, 0, 1, 2);
    partLayout->addWidget(nickLabel, 2, 0);
    partLayout->addWidget(messageEdit, 2, 1);
    partLayout->addWidget(debuggerLabel, 4, 0, 1, 2);
    partLayout->addWidget(buttonBox, 5, 0, 1, 2);
    partLayout->addWidget(messageList, 6, 0, 3, 2);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(partLayout);
    mainLayout->addWidget(playerList);
    setLayout(mainLayout);

    setWindowTitle(tr("ZAG client"));

    //End UI part */

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

Client::~Client(){
    delete statusLabel;
    delete hostLabel;
    delete portLabel;
    delete hostEdit;
    delete portEdit;
    delete sendMessageButton;
    delete quitButton;
    delete connectToHostButton;
    delete buttonBox;
    delete debuggerLabel;
    delete messageEdit;
    delete nickLabel;
    delete messageList;
}

void Client::connectToHost(){
    tcpSocket -> connectToHost(hostEdit -> currentText(), port);
}

void Client::setHostAndPort(){
    //hostName = hostEdit -> currentText();
    port = (portEdit -> text()).toInt();
}


//This is for sending a new message to other players
//Called when sendMessage button is pressed
void Client::sendMessage(){
    QString str;
    str += nickLabel->text();
    str += ": ";
    str += messageEdit->text();

    writeString(str);
}

/*
 *This is called when a new message is sent to client before the game begins
 *It will specify different kinds of messages and work accordingly
 *Note that this will not be called once the game starts
 */
void Client::readMessage(){
    //qDebug("Old version of receive");
    //statusLabel -> setText(tr("Message got!"));

    if(!readCheck())
        return;

    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);
    quint16 mode;
    in >> mode;

    //These constants is defined in BattleField.h and shared with server
    switch(mode){
    case GREETING:
        sendNickName();
        break;
    case MESSAGE:
        displayMessage();
        break;
    case PLAYERS:
        displayPlayers();
        break;
    case GAME_BEGIN:
        prepareGame();
        break;
    }

}

//Show the error message when connection to the server
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

//Used for network session, which is the basics for network in Qt.
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

//This will write a message contains a string to server
//Information attached is used for completeness
void Client::writeString(QString str){
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    out << quint32(0);
    out << str;
    out.device()->seek(0);
    out << quint32(block.size() - sizeof(quint32));
    tcpSocket->write(block);
}

/*
 *This function is called whenever client want to read from socket
 *Based on protocal designed, this system will promise the completeness of the information transformed
 *
 */
bool Client::readCheck(){
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);

    if(blockSize == 0){
        if(tcpSocket->bytesAvailable() < (int) sizeof(quint32))
            return false;

        in >> blockSize;
    }

    if(tcpSocket->bytesAvailable() < blockSize)
        return false;

    blockSize = 0;
    return true;
}

//Called after the connection application is received by the server.
void Client::sendNickName(){
    QString name = messageEdit->text();
    writeString(name);

    nickLabel->setText(name + ":");
    sendMessageButton->setEnabled(true);
    connectToHostButton->setEnabled(false);
}

//Called when a message is received
void Client::displayMessage(){
    //tcpSocket->write(*setMessage());
    //statusLabel->setText(nextMessage);
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);
    QString nextMessage;
    in >> nextMessage;
    QStringList mesList = nextMessage.split("#", QString::SkipEmptyParts);
    foreach(QString mes, mesList){
        messageList->addItem(mes);
    }
}

//Called when a list of temporary playes is received, which will be sent everytime a new play joins
void Client::displayPlayers(){
    QDataStream in(tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);
    QString nextMessage;
    in >> nextMessage;
    QStringList nameList = nextMessage.split('@', QString::SkipEmptyParts);
    playerList->clear();
    foreach(QString name, nameList){
        playerList->addItem(name);
    }
}

//***************************************
//Following is the game part

/*
 *When a game begin signal is sent, this function will be called
 *This function prepare the client for the game and start the communication to server after 500 ms
 *The interval is aimed at promising the communication will be started
 *
 */
void Client::prepareGame(){
    //tcpSocket->readAll();
    disconnect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));

    bf = new BattleField;
    //rule = new BFRShoot(bf->getManager());
    //bf->getManager()->setRule(rule);
    connect(bf, SIGNAL(battleEnd()), this, SLOT(battleEnd()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(clientGameUpdate()));

    int frac = getFraction();
    bf->getManager()->decodeReplaceAllObjects(tcpSocket);
    std::set<BFObject *> objList = bf->getManager()->getObjects();
    std::vector<BFObjectID> fracObj;
    for(std::set<BFObject*>::iterator ite = objList.begin(); ite != objList.end(); ite++){
        if((**ite)["fraction"].toInt() == frac){
            fracObj.push_back((*ite)->getID());
        }
    }

    BFCHumanAndRSD *ctr = new BFCHumanAndRSD(bf->getManager(), fracObj);
    //BFCHuman *ctr = new BFCHuman(bf->getManager(), fracObj);
    bf->getManager()->registerController(ctr);
    bf->setFocusObject(bf->getManager()->getObjectControlled()->getID());

    //qDebug("Prepare finished");
    bf->update();
    //write("Know");
    QTimer::singleShot(500, this, SLOT(sendAck()));

    bf->show();
    this->hide();

    blockSize = 0;
}

//This will be called whenever a message is received
void Client::clientGameUpdate(){
    if(!readCheck())
        return;

    bf->getManager()->destructObjects();
    bf->getManager()->decodeReplaceAllObjects(tcpSocket);
    bf->update();
    //qDebug("#objects = %d", bf->getManager()->getObjects().size());

    sendReturnMessage();
}

//This is called when the player exit the game
void Client::battleEnd(){
    disconnect(tcpSocket, SIGNAL(readyRead()), this, SLOT(clientGameUpdate()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    delete bf;
    //delete rule;

    this->show();
}

//This will send the control information back to user
void Client::sendReturnMessage(){
    //qDebug("Sending Control Info on Client");
    QByteArray message;
    QDataStream out(&message, QIODevice::WriteOnly);
    out << quint32(0);

    bf->getManager()->processInput();
    std::vector<ControlEvent> controlList = getAllControls();
    ControlEvent::encodeControlEventList(controlList, out.device());

    out.device()->seek(0);
    out << quint32(message.size() - sizeof(quint32));

    tcpSocket->write(message);
}

//This will start the communication
void Client::sendAck(){
    //qDebug("ACK sent");
    sendReturnMessage();
}

//This is used to get the user's input info
std::vector<ControlEvent> Client::getAllControls(){
    std::vector<ControlEvent> events;
    std::set<BFController *> &controllers = bf->getManager()->getControllers();
    std::set<BFController *>::iterator ctrliter;
    for (ctrliter = controllers.begin(); ctrliter != controllers.end(); ctrliter++)
    {
        //(*ctrliter)->applyControl();
        std::vector<ControlEvent> temVec = (*ctrliter)->getControl();
        std::vector<ControlEvent>::iterator ite = temVec.begin();
        for(; ite != temVec.end(); ite++){
            events.push_back(*ite);
        }
    }

    qDebug("Objects = %d", (int)bf->getManager()->getObjects().size());
    for(std::vector<ControlEvent>::iterator ite = events.begin(); ite != events.end(); ite++){
        qDebug("The acc is: %lf, %lf", (*ite).acc.x, (*ite).acc.y);
    }
/*
    std::vector<ControlEvent> decodeVec;
    QByteArray tem;
    QBuffer buf(&tem);
    buf.open(QIODevice::ReadWrite);
    ControlEvent::encodeControlEventList(events, &buf);
    buf.seek(0);
    ControlEvent::decodeAppendControlEventList(decodeVec, &buf);
    for(std::vector<ControlEvent>::iterator ite = decodeVec.begin(); ite != decodeVec.end(); ite++){
        qDebug("The decode version of acc is: %lf, %lf", (*ite).acc.x, (*ite).acc.y);
    }
*/
    return events;
}

//This is used to get what fraction the user belongs to, which is a game info.
int Client::getFraction(){
    quint16 frac;
    QDataStream in(tcpSocket);
    in >> frac;
    return int(frac);
}
