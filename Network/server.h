#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QTcpServer>
#include <QNetworkSession>

class Server: public QDialog
{
    Q_OBJECT

public:
    Server(QWidget *parent = 0);

private slots:
    void sessionOpened();
    void sendMessage();
    QStringList encodeMessage();

private:
    QTcpServer *tcpServer;
    QStringList messages;
    QNetworkSession *networkSession;
};

#endif // SERVER_H
