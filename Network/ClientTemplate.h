#ifndef __CLIENT_TEMPLATE_H__
#define __CLIENT_TEMPLATE_H__

#include <QDialog>
#include <QTcpServer>
#include "client.h"

QT_BEGIN_NAMESPACE
class QTcpSocket;
class QNetworkSession;
class QLabel;
class QLineEdit;
class QPushButton;
class QDialogButtonBox;
class QComboBox;
QT_END_NAMESPACE

class ClientTemplate: public QDialog{
    Q_OBJECT

public:
    ClientTemplate(QWidget *parent = 0);

private slots:
    void gameBegin();
    void sendMessage();
    QDataStream &getMessage();
    void setHostAndPort();

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

    Client *client;
    //QByteArray message;
};

#endif //__CLIENT_TEMPLATE_H__
