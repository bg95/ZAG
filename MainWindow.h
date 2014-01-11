#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class BattleField;
class Server;
class Client;

#include "BattleField/BattleField.h"
#include "BattleField/BFRule/BFRCollision.h"
#include "Network/Client.h"
#include "Network/Server.h"

#include <QMainWindow>
#include <QPushButton>

#define mainWindowWidth 500.0
#define mainWindowHeight 500.0
#define mainWindowLUx 20.0//left upper x coordiante
#define mainWindowLUy 20.0//left upper y coordinate

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget//public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
protected:
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;
    QPushButton *serverButton;
    QPushButton *clientButton;
    QPushButton *singleButton;
    QPushButton *quitButton;
    Client *client;
    Server *server;
public: //for debugging
    BattleField *bf;
    BFRule *rule;
private slots:
    void serverMode();
    void clientMode();
    void singlePlayer();
    void battleEnd();
};

#endif // MAINWINDOW_H
