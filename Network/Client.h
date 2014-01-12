#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QTcpSocket>
#include "BattleField/BattleField.h"
#include "BattleField/BFRule/BFRCollision.h"

QT_BEGIN_NAMESPACE
class QTcpSocket;
class QNetworkSession;
class QLabel;
class QLineEdit;
class QPushButton;
class QDialogButtonBox;
class QComboBox;
class QListWidget;
QT_END_NAMESPACE

class Client : public QDialog{
    Q_OBJECT

public:
    Client(QWidget *parent = 0);
    ~Client();

private slots:
    void connectToHost();
    void sendMessage();
    //void requestNewMessage();
    void readMessage();
    void displayError(QAbstractSocket::SocketError);
    void sessionOpened();
    void battleEnd();

    void clientGameUpdate();
    void sendAck();

    //This is for test
    void setHostAndPort();
    //End test for*/

private:
    //This is for test
    QLabel *nickLabel;
    QLabel *debuggerLabel;
    QLabel *statusLabel;
    QLabel *hostLabel;
    QLabel *portLabel;
    QComboBox *hostEdit;
    QLineEdit *portEdit;
    QLineEdit *messageEdit;
    QPushButton *sendMessageButton;
    QPushButton *quitButton;
    QPushButton *connectToHostButton;
    QDialogButtonBox *buttonBox;
    QListWidget *playerList;
    QListWidget *messageList;

    //QByteArray *setMessage();
    //End test part */

    //QHostAddress hostName;
    quint16 port;

    QTcpSocket *tcpSocket;
    //QByteArray *message;
    //QString currentMessage;
    quint32 blockSize;

    QNetworkSession *networkSession;

    BattleField *bf;
    BFRule *rule;

    void writeString(QString);
    void sendReturnMessage();
    std::vector<ControlEvent> getAllControls();

    int counter; //counter used for test network complecity
};


#endif // CLIENT_H
