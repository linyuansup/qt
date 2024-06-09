#include "client.h"

Client::Client(QObject* parent) : QTcpSocket(parent)
{
	Q_UNUSED(parent)
		connect(this, &Client::readyRead, this, &Client::dataReceived);
	connect(this, &Client::disconnected, this, &Client::slotDisconnected);
}

void Client::dataReceived()
{
	while (bytesAvailable() > 0)
	{
		int length = bytesAvailable();
		char buf[1024];
		read(buf, length);

		QString msg = buf;
		emit updateClients(msg, length);
	}

}

void Client::slotDisconnected()
{
	emit disconnected(this->socketDescriptor());
}
