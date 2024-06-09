#include "main_window.h"
#include "client.h"

#include <QApplication>
#include <QHostAddress>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	//MainWindow w;
	//w.show();
	Client client;
	auto socket = client.connectTo(QHostAddress("127.0.0.1").toIPv4Address(), 8888);
	auto port = socket->socket->localPort();
	//client.sendText(socket, "Hello", QHostAddress("127.0.0.1").toIPv4Address(), port);
	//client.sendText(socket, "Hello", QHostAddress("127.0.0.1").toIPv4Address(), port);
	//client.sendText(socket, "Hello", QHostAddress("127.0.0.1").toIPv4Address(), port);
	//client.sendText(socket, "Hello", QHostAddress("127.0.0.1").toIPv4Address(), port);
	//client.sendText(socket, "Hello", QHostAddress("127.0.0.1").toIPv4Address(), port);
	//client.sendText(socket, "Hello", QHostAddress("127.0.0.1").toIPv4Address(), port);
	auto file = new QFile("C:/Users/93601/Desktop/1.wav");
	auto file2 = new QFile("C:/Users/93601/Desktop/2.wav");
	file->open(QIODevice::ReadOnly);
	file2->open(QIODevice::ReadOnly);
	client.sendText(socket, "Hello", QHostAddress("127.0.0.1").toIPv4Address(), port);
	client.sendFile(socket, file, QHostAddress("127.0.0.1").toIPv4Address(), port);
	client.sendText(socket, "Hello", QHostAddress("127.0.0.1").toIPv4Address(), port);
	client.sendFile(socket, file2, QHostAddress("127.0.0.1").toIPv4Address(), port);
	return a.exec();
}
