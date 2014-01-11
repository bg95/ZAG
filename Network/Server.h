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

QT_BEGIN_NAMESPACE
class QTcpSocket;
class QLabel;
class QPushButton;
class QLineEdit;
class QTcpServer;
class QNetworkSession;
class QListWidget;
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
    void updateClient(QByteArray);
    void newMessage();
    void sendMessage();
    //void encodeMessage();

private:
    QTcpServer *tcpServer;
    //QString messages;
    QNetworkSession *networkSession;
    QTcpSocket *clientConnection;
    QList<QTcpSocket*> connectionList;
    QList<QString> nickNameList;
    QTimer *networkTimer;

    quint16 blockSize;
    QString currentMessageGot;

    bool gameOn;
    BattleField *bf;
    BFRule *bfRule;

    QByteArray *getMessage();
    //void sendMessage(QTcpSocket *connection);
    void prepareInitialState();
    QByteArray writeString(QString str);

    //This part is for test
    QLabel *debuggerLabel;
    QLabel *statusLabel;
    QLineEdit *sentMessage;
    QPushButton *quitButton;
    QPushButton *gameBeginButton;
    QPushButton *sendMessageButton;
    QListWidget *participantList;
    QListWidget *messageList;
};

#endif // SERVER_H
