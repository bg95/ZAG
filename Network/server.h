#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
//#include <QTcpServer>
//#include <QNetworkSession>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QTcpServer;
class QNetworkSession;
QT_END_NAMESPACE

class Server: public QDialog
{
    Q_OBJECT

public:
    Server(QWidget *parent = 0);

private slots:
    void sessionOpened();
    void sendMessage();
    QString encodeMessage();

private:
    QTcpServer *tcpServer;
    QString messages;
    QNetworkSession *networkSession;

    //This part is for test
    QLabel *debuggerLabel;
    QLabel *statusLabel;
    QPushButton *quitButton;
};

#endif // SERVER_H
