#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
class QTcpSocket;
class QNetworkSession;
QT_END_NAMESPACE

class Client : public QDialog{
    Q_OBJECT

public:
    Client(QWidget *parent = 0);

private slots:
    void requestNewMessage();
    void readMessage();
    void displayError(QAbstractSocket::SocketError);
    void sessionOpened();

private:
    QString hostName;
    quint16 port;

    QTcpSocket *tcpSocket;
    QStringList messageList;
    QString currentMessage;
    quint16 blockSize;

    QNetworkSession *networkSession;
};


#endif // CLIENT_H
