#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QList>
#include <QTimer>
//#include <QString>
//#include <QTcpServer>
//#include <QNetworkSession>

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
    QTimer *networkTimer;
    bool flag;

    quint32 blockSize;
    //QString currentMessageGot;

    bool gameOn;
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

    int counter; //Counter used for test network complecity

    //This part is for test
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
