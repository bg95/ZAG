#include "MainWindow.h"
#include <QApplication>

#include "main.h"
MainWindow *mw;

BFOCircle *circles[10];

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    mw = &w;
    w.show();
    
    return a.exec();
}
