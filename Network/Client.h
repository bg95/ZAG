#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
class QTcpSocket;
class QNetworkSession;
class QLabel;
class QLineEdit;
class QPushButton;
class QDialogButtonBox;
class QComboBox;
QT_END_NAMESPACE

class Client : public QDialog{
    Q_OBJECT

public:
    Client(QWidget *parent = 0);
    ~Client();

private slots:
    void connectToHost();
    //void sendMessage(QString);
    void requestNewMessage();
    void readMessage();
    void displayError(QAbstractSocket::SocketError);
    void sessionOpened();

    //This is for test
    void setHostAndPort();
    //End test for*/

private:
    //This is for test
    QLabel *debuggerLabel;
    QLabel *statusLabel;
    QLabel *hostLabel;
    QLabel *portLabel;
    QComboBox *hostEdit;
    QLineEdit *portEdit;
    QLineEdit *messageEdit;
    QPushButton *getMessageButton;
    QPushButton *quitButton;
    QPushButton *connectToHostButton;
    QDialogButtonBox *buttonBox;
    //End test part */

    //QHostAddress hostName;
    quint16 port;

    QTcpSocket *tcpSocket;
    QStringList messageList;
    QString currentMessage;
    quint16 blockSize;

    QNetworkSession *networkSession;
};


#endif // CLIENT_H
