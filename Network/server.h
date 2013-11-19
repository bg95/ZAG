#ifndef SERVER_H
#define SERVER_H

#include <QDialog>

class server: public QDialog
{
    Q_OBJECT

public:
    Server(QWidget *parent = 0);

private slots:
    void sessionOpended();
    void sendMessage();

private:
    QTcpServer *tcpServer;
    QStringList messages;
    QNetworkSession *networkSession;
};

#endif // SERVER_H
