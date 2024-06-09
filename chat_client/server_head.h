#ifndef SERVER_HEAD_H
#define SERVER_HEAD_H

#include <QHostAddress>

class ServerHead : public QObject {

	Q_OBJECT

public:
	char type;
	QHostAddress address;
	unsigned short port;
	unsigned long long messageSize;
};

#endif // SERVER_HEAD_H
