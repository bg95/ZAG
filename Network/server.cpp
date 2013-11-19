#include <QtWidgets>
#include <QtNetwork>

#include "server.h"

Server::Server(Qwidget *parent):
    QDialog(parent), tcpServer(0), networkSession(0){

    QNetworkConfigurationManager manager;
    if(manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired){
        QSettings settings(QSettings::UserScope, QLatinString("QtObject"));
    }
}
