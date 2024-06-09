#ifndef CLIENT_H
#define CLIENT_H

#include <QFile>
#include <QTcpServer>
#include <QStandardPaths>

#include "socket.h"
#include "server_head.h"
#include "text_message.h"
#include "socket_list.h"
#include "QFileDialog"

class Client : public QObject {
	Q_OBJECT

public:
	SocketList socketList;

public:
	Client();
	~Client();
	void newConnection(Socket* socket);
	void sendFile(Socket* socket, QFile* file, unsigned int targetIp, unsigned short targetPort);
	void sendText(Socket* socket, QString text, unsigned int targetIp, unsigned short targetPort);
	void startListen(unsigned short port);
	Socket* connectTo(unsigned int address, unsigned short port);
};

#endif // CLIENT_H
