/*Brief description for CLIENT part
 *
 *This file defines the client part.
 *Client part can be seen as participant of the game. It will connect to a server to join a desired game and
 *you can choose a nick name as you wish.
 *Before the game is started, you can chat with other players.
 *Client part does not do much calculation when the game begins and it depends on info transmitted by the server.
 *In order to keep communication complete, a special protocal is designed for communication,
 *which will promise every time what server receives is complete.
 *
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QTcpSocket>
#include "BattleField/BattleField.h"
#include "BattleField/BFRule/BFRCollision.h"
//#include "global.h"

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

    void setHostAndPort();

private:
    //This UI part
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

    //This is network part
    //QHostAddress hostName;
    quint16 port;
    QTcpSocket *tcpSocket;
    //QByteArray *message;
    //QString currentMessage;
    quint32 blockSize;
    QNetworkSession *networkSession;

    //This is game part
    BattleField *bf;
    BFRule *rule;

    void writeString(QString);
    bool readCheck();
    void sendReturnMessage();
    void sendNickName();
    void displayMessage();
    void displayPlayers();
    void prepareGame();
    int getFraction();
    std::vector<ControlEvent> getAllControls();

    int counter;        //counter used for test network complecity
};


#endif // CLIENT_H
