#include "main_window.h"
#include "client.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
	Client client;
	client.startListen(8888);
    return a.exec();
}
