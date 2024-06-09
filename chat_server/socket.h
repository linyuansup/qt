#ifndef SOCKET_H
#define SOCKET_H

#include <QTcpSocket>
#include <QByteArray>
#include "byte_array.h"
#include "server_head.h"
#include "text_message.h"
#include "file_head.h"

class Socket : public QObject {

	Q_OBJECT

public:
	QTcpSocket* socket = nullptr;
	QByteArray* buffer = nullptr;
	ServerHead* head = nullptr;
	bool serverMode;
	Socket* targetSocket = nullptr;
	bool hasSendHead = false;
	FileHead* fileHead = nullptr;
	unsigned long long forwarded = 0;

public:
	Socket(QTcpSocket* socket, bool serverMode);
	void readHeader();
	void forward();
	void write(const void* data, unsigned long long size);
	unsigned int getPeerAddress();
	unsigned short getPeerPort();
	~Socket();

signals:
	void prepareForward();
signals:
	void messageReceived(TextMessage* message);
signals:
	void fileRecieved(ServerHead* server, FileHead* message, QByteArray* content);
signals:
	void disconnected();
signals:
	void onProcess(ServerHead* server, FileHead* message, unsigned long long readed, unsigned long long total);
};

#endif // SOCKET_H
