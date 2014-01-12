#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <QString>

QT_BEGIN_NAMESPACE
class QTcpSocket;
QT_END_NAMESPACE

class Connection
{
public:
    Connection(QTcpSocket *c);
    QTcpSocket *getConnection();
    bool permission();
    void open();
    void close();
    QString getNickName();
    void setNickName(QString nick);

private:
    QTcpSocket *connection;
    bool openFlag;
    QString nickName;
};

#endif //__CONNECTION_H__
