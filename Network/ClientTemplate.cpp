#include <QtWidgets>
#include <QtNetwork>

#include "ClientTemplate.h"

ClientTemplate::ClientTemplate(QWidget *parent): QDialog(parent){
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

    connect(getMessageButton, SIGNAL(clicked()), this, SLOT(gameBegin));
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

    client = new Client(parent);
}

void ClientTemplate::gameBegin(){
}

void ClientTemplate::setHostAndPort(){
    client -> setPort((portEdit -> text()).toInt());
    client -> setHost(hostEdit -> currentText());
}

void ClientTemplate::sendMessage(QByteArray message){
    client -> sendMessage(message);
}

QDataStream &ClientTemplate::getMessage(){
    return client -> getMessage();
}
