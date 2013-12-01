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

private slots:
    void requestNewMessage();
    void readMessage();
    void displayError(QAbstractSocket::SocketError);
    void sessionOpened();

    //This is for test
    void setHostAndProt();
    //End test for*/

private:
    //This is for test
    QLabel *statusLabel;
    QLabel *hostLabel;
    QLabel *portLabel;
    QComboBox *hostEdit;
    QLineEdit *portEdit;
    QPushButton *getMessageButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;
    //End test part */

    QString hostName;
    quint16 port;

    QTcpSocket *tcpSocket;
    QStringList messageList;
    QString currentMessage;
    quint16 blockSize;

    QNetworkSession *networkSession;
};


#endif // CLIENT_H
