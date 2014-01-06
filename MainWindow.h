#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "BattleField/BattleField.h"
#include "BattleField/BFRule/BFRCollision.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
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
public: //for debugging
    BattleField *bf;
    BFRule *rule;
};

#endif // MAINWINDOW_H
