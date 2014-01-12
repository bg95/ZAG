/*Brief description of the SERVER part
 *
 *This file, along with Server.cpp, defines the server part of the network.
 *When start a network version, a server must be started. Server will manage the connections offerred by clients.
 *Besides, most calculation when game is started is done by server and server is also responsible for
 *synchronizing all clients.
 *
 */

#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QList>
#include <QTimer>

#include "BattleField/BattleField.h"
#include "BattleField/BFRule/BFRCollision.h"
#include "Network/Connection.h"
//#include "global.h"

QT_BEGIN_NAMESPACE
class QTcpSocket;
class QLabel;
class QPushButton;
class QLineEdit;
class QTcpServer;
class QNetworkSession;
class QListWidget;
class QComboBox;
QT_END_NAMESPACE

class Server: public QDialog
{
    Q_OBJECT

public:
    Server(QWidget *parent = 0);
    ~Server();

private slots:
    void acceptConnection();
    void auth();
    void sessionOpened();
    void gameBegin();
    //void updateNetwork();
    void battleEnd();
    void updateClient();
    void newMessage();
    void sendMessage();
    void newInterval();
    //void encodeMessage();

private:
    QTcpServer *tcpServer;
    QByteArray lastMessage;
    QNetworkSession *networkSession;
    QTcpSocket *clientConnection;

    QList<Connection*> connectionList;
    //QList<QString> nickNameList;
    //QTimer *networkTimer;
    bool flag;          //flag used to limit the rake of tranmission
    quint32 blockSize;
    //QString currentMessageGot;

    bool gameOn;        //Used to hold new connection when the game is started
    BattleField *bf;
    BFRule *bfRule;
    int numberOfConnections;

    QByteArray *getMessage();
    //void sendMessage(QTcpSocket *connection);
    void prepareInitialState();
    void updateClientControl(QTcpSocket *);
    bool checkConnectionNumber();
    QByteArray prepareSendMessage();
    QByteArray writeString(QString str, const quint16 mode);
    bool readCheck(QTcpSocket *cli);
    void sendGreetingMessage(QTcpSocket *cli);
    QByteArray sendInitialMessage();

    int counter;            //Counter used for test network complecity

    //This is the UI part
    QLabel *debuggerLabel;
    QLabel *statusLabel;
    QLineEdit *sentMessage;
    QPushButton *quitButton;
    QPushButton *gameBeginButton;
    QPushButton *sendMessageButton;
    QListWidget *participantList;
    QListWidget *messageList;
    //QComboBox *sideChoiceList;
};

#endif // SERVER_H
