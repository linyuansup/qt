#ifndef SOCKET_LIST_H
#define SOCKET_LIST_H

#include <vector>
#include <QMutex>
#include "socket.h"

class SocketList : QObject {
	Q_OBJECT
public:
	std::vector<Socket*> vec;
	QMutex mutex;

public:
	SocketList();
	~SocketList();
	void add(Socket* socket);
	void remove(Socket* socket);
	Socket* find(unsigned int address, unsigned short port);
};

#endif // SOCKET_LIST_H
