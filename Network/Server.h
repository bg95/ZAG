#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
//#include <QTcpServer>
//#include <QNetworkSession>

QT_BEGIN_NAMESPACE
class QTcpSocket;
class QLabel;
class QPushButton;
class QLineEdit;
class QTcpServer;
class QNetworkSession;
QT_END_NAMESPACE

class Server: public QDialog
{
    Q_OBJECT

public:
    Server(QWidget *parent = 0);
    ~Server();

private slots:
    void acceptConnection();
    void sessionOpened();
    void sendMessage();
    void encodeMessage();

private:
    QTcpServer *tcpServer;
    QString messages;
    QNetworkSession *networkSession;
    QTcpSocket *clientConnection;

    quint16 blockSize;
    QString currentMessageGot;

    //This part is for test
    QLabel *debuggerLabel;
    QLabel *statusLabel;
    QLineEdit *sentMessage;
    QPushButton *quitButton;
};

#endif // SERVER_H
