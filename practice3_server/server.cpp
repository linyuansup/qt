#include "server.h"

Server::Server(int port, QObject* parent) : QTcpServer(parent)
{
	listen(QHostAddress::Any, port);
}

void Server::updateClients(QString msg, int length)
{
	emit updateServer(msg, length);
	for (int i = 0; i < clientList.count(); i++)
	{
		QTcpSocket* item = clientList.at(i);
		QByteArray str = msg.toUtf8();
		item->write(str, length);
	}
}

void Server::slotDisconnected(int descriptor)
{
	for (int i = 0; i < clientList.count(); i++)
	{
		QTcpSocket* item = clientList.at(i);
		if (item->socketDescriptor() == descriptor)
		{
			delete item;
			clientList.removeAt(i);
			return;
		}
	}
	return;
}

void Server::incomingConnection(qintptr socketDescriptor)
{
	Client* tcpClientSocket = new Client(this);
	connect(tcpClientSocket, &Client::updateClients, this, &Server::updateClients);
	connect(tcpClientSocket, &Client::disconnected, this, &Server::slotDisconnected);

	tcpClientSocket->setSocketDescriptor(socketDescriptor);

	clientList.append(tcpClientSocket);

}
