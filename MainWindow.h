#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "BattleField/BattleField.h"
#include "BattleField/BFRule/BFRCollision.h"

#include <QMainWindow>
#include <QPushButton>

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

private:
    Ui::MainWindow *ui;
    QPushButton *serverButton;
    QPushButton *clientButton;
    QPushButton *singleButton;
    QPushButton *quitButton;
public: //for debugging
    BattleField *bf;
    BFRule *rule;
private slots:
    void server();
    void client();
    void singlePlayer();
};

#endif // MAINWINDOW_H
