#include "MainWindow.h"
#include <QApplication>

#include "main.h"

#include "Network/Server.h"
#include "Network/Client.h"

MainWindow *mw;

BFOCircle *circles[10];

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /*This is for test
    Server server;
    server.show();
    //This is for test*/
    MainWindow w;
    mw = &w;
    w.show();
    return a.exec();
}
