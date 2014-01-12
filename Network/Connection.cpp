#include <QtNetwork>

#include "Network/Connection.h"

Connection::Connection(QTcpSocket *c): connection(c), openFlag(true), nickName("Unknown"){
}

QTcpSocket *Connection::getConnection(){
    return connection;
}

bool Connection::permission(){
    return openFlag;
}

void Connection::open(){
    openFlag = true;
}

void Connection::close(){
    openFlag = false;
}

QString Connection::getNickName(){
    return nickName;
}

void Connection::setNickName(QString nick){
    nickName = nick;
}

void Connection::setCircleID(int id){
    circleID = id;
}
