#ifndef CLIENT_H
#define CLIENT_H

#include <QFile>
#include <QStandardPaths>
#include <QMessageBox>
#include <QTcpServer>

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

signals:
	void messageReceived(TextMessage* message);
signals:
	void onProcess(ServerHead* server, FileHead* message, unsigned long long readed, unsigned long long total);
signals:
	void disconnected();
signals:
	void fileReceived(ServerHead* server, FileHead* message, QByteArray* content);
};

#endif // CLIENT_H
