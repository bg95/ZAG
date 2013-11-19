#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
class QTcpSocket;
class QNetworkSession;
QT_END_NAMESPACE

class Client : public Qdialog{
    Q_OBJECT

public:
    Client(QWidget *parent = 0);

private slots:
    void requestNewMessage();
    void readMessage();
    void displayError(QAbstractSocket::SocketError)
};


#endif // CLIENT_H
