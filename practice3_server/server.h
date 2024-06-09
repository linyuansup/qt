#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QObject>
#include "client.h"

class Server : public QTcpServer
{
	Q_OBJECT

public:
	explicit Server(int port, QObject* parent = nullptr);
	QList<Client*> clientList;

signals:
	void updateServer(QString, int);

public slots:
	void updateClients(QString, int);
	void slotDisconnected(int);

protected:
	void incomingConnection(qintptr socketDescriptor);

};

#endif // SERVER_H
