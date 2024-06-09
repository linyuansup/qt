#include "client.h"

Client::Client()
{
}

Client::~Client()
{
}

void Client::newConnection(Socket* socket)
{
	socketList.add(socket);
	connect(socket, &Socket::prepareForward, [this, socket] {
		auto target = socketList.find(socket->head->address.toIPv4Address(), socket->head->port);
		if (target == nullptr)
		{
			qDebug() << "Target not found";
			return;
		}
		socket->targetSocket = target;
		});
	connect(socket, &Socket::messageReceived, [this](TextMessage* message) {
		qDebug() << "Recieve message from " + message->headInfo->address.toString() + ":" + QString::number(message->headInfo->port);
		qDebug() << "Content: " + message->message;
		emit messageReceived(message);
		});

	connect(socket, &Socket::fileRecieved, [this](ServerHead* server, FileHead* message, QByteArray* content) {
		QString path = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation) + "/" + message->filename;
		QFile file(path);
		file.open(QIODevice::WriteOnly);
		file.write(content->data(), content->size());
		file.close();
		emit fileReceived(server, message, content);
		});

	connect(socket, &Socket::disconnected, [this, socket] {
		qDebug() << "Client ip = " + socket->socket->peerAddress().toString() + " disconnected";
		emit disconnected();
		});

	connect(socket, &Socket::onProcess, [this](ServerHead* server, FileHead* message, unsigned long long readed, unsigned long long total) {
		qDebug() << message->filename + " readed: " + QString::number(readed) + " Total: " + QString::number(total);
		emit onProcess(server, message, readed, total);
		});
}

void Client::sendFile(Socket* socket, QFile* file, unsigned int targetIp, unsigned short targetPort)
{
	auto fileContent = file->readAll();
	auto filename = file->fileName();
	auto filenameUtf = filename.right(filename.size() - filename.lastIndexOf("/") - 1).toUtf8();
	unsigned long long fileContentLength = fileContent.size();
	unsigned long long filenameLength = filenameUtf.size();
	auto type = 'F';
	socket->write(&type, 1);
	socket->write(&targetIp, 4);
	socket->write(&targetPort, 2);
	unsigned long long messageSize = fileContentLength + filenameLength + 8;
	socket->write(&messageSize, 8);
	socket->write(&filenameLength, 8);
	socket->write(filenameUtf.constData(), filenameLength);
	socket->write(fileContent.constData(), fileContentLength);
}

void Client::sendText(Socket* socket, QString text, unsigned int targetIp, unsigned short targetPort)
{
	auto message = text.toUtf8();
	unsigned long long messageSize = message.size();
	auto type = 'T';
	socket->write(&type, 1);
	socket->write(&targetIp, 4);
	socket->write(&targetPort, 2);
	socket->write(&messageSize, 8);
	socket->write(message.constData(), messageSize);
}

void Client::startListen(unsigned short port)
{
	auto server = new QTcpServer();
	connect(server, &QTcpServer::newConnection, [server, this] {
		auto socket = server->nextPendingConnection();
		newConnection(new Socket(socket, true));
		});
	server->listen(QHostAddress::Any, port);
}

Socket* Client::connectTo(unsigned int address, unsigned short port)
{
	auto tcpSocket = new QTcpSocket();
	tcpSocket->connectToHost(QHostAddress(address), port);
	auto socket = new Socket(tcpSocket, false);
	newConnection(socket);
	return socket;
}
