#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class BattleField;

#include "BattleField/BattleField.h"
#include "BattleField/BFRule/BFRCollision.h"

#include <QMainWindow>

#define mainWindowWidth 500.0
#define mainWindowHeight 500.0
#define mainWindowLUx 20.0//left upper x coordiante
#define mainWindowLUy 20.0//left upper y coordinate

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
