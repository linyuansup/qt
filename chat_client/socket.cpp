#include "socket.h"

Socket::Socket(QTcpSocket* socket, bool serverMode) : socket(socket), serverMode(serverMode)
{
	qDebug() << "Socket port is " + QString::number(socket->peerPort());
	connect(socket, &QTcpSocket::readyRead, [this, socket, serverMode] {
		if (buffer == nullptr) {
			buffer = new QByteArray();
		}
		auto data = socket->readAll();
		buffer->append(data);
		while (buffer != nullptr && buffer->size() != 0) {
			if (head == nullptr) {
				if (buffer->size() < 15) {
					return;
				}
				readHeader();
			}
			if (serverMode && buffer->size() > 0) {
				if (targetSocket == nullptr) {
					emit prepareForward();
				}
				forward();
				return;
			}
			if (buffer->size() > 0) {
				if (head->type == 'T') {
					if (buffer->size() < head->messageSize) {
						return;
					}
					auto message = new TextMessage();
					message->headInfo = head;
					auto content = buffer->mid(0, head->messageSize);
					message->message = ByteArray(&content).asString();
					emit messageReceived(message);
					if (buffer->size() == head->messageSize) {
						buffer = nullptr;
					}
					else {
						buffer = new QByteArray(buffer->mid(head->messageSize));
					}
					head = nullptr;
				}
				else if (head->type == 'F' && fileHead == nullptr) {
					if (buffer->size() < 8) {
						return;
					}
					fileHead = new FileHead();
					auto content = buffer->mid(0, 8);
					fileHead->fileNameLength = ByteArray(&content).as64Bits();
					buffer = new QByteArray(buffer->mid(8));
				}
				if (fileHead != nullptr && buffer != nullptr) {
					if (fileHead->filename.isEmpty()) {
						if (buffer->size() < fileHead->fileNameLength) {
							return;
						}
						auto content = buffer->mid(0, fileHead->fileNameLength);
						fileHead->filename = ByteArray(&content).asString();
						buffer = new QByteArray(buffer->mid(fileHead->fileNameLength));
					}
					emit onProcess(fileHead->filename, std::min((unsigned long long)buffer->size(), head->messageSize), head->messageSize);
					if (buffer->size() >= head->messageSize - 8 - fileHead->filename.length()) {
						if (buffer->size() == head->messageSize - 8 - fileHead->filename.length()) {
							emit fileRecieved(fileHead, buffer);
							buffer = nullptr;
							return;
						}
						else {
							auto fileContent = buffer->mid(0, head->messageSize - 8 - fileHead->filename.length());
							emit fileRecieved(fileHead, &fileContent);
							buffer = new QByteArray(buffer->mid(head->messageSize - 8 - fileHead->filename.length()));
						}
						fileHead = nullptr;
						head = nullptr;
					}
					else {
						return;
					}
				}
			}
		}
		});

	connect(socket, &QTcpSocket::disconnected, [this] {
		emit disconnected();
		});
}

void Socket::readHeader()
{
	head = new ServerHead();
	head->type = buffer->at(0);
	auto data = buffer->mid(1, 4);
	head->address = QHostAddress(ByteArray(&data).as32Bits());
	data = buffer->mid(5, 2);
	head->port = ByteArray(&data).as16Bits();
	data = buffer->mid(7, 8);
	head->messageSize = ByteArray(&data).as64Bits();
	buffer = new QByteArray(buffer->mid(15));
}

void Socket::forward()
{
	if (targetSocket != nullptr) {
		if (!hasSendHead) {
			targetSocket->write(&head->type, 1);
			unsigned int ip = getPeerAddress();
			targetSocket->write(&ip, 4);
			unsigned short port = getPeerPort();
			targetSocket->write(&port, 2);
			targetSocket->write(&head->messageSize, 8);
			hasSendHead = true;
		}
		targetSocket->write(buffer->data(), buffer->size());
	}
	forwarded += buffer->size();
	if (forwarded == head->messageSize) {
		head = nullptr;
		hasSendHead = false;
	}
	buffer = nullptr;
}

void Socket::write(const void* data, unsigned long long size)
{
	socket->write((char*)data, size);
}

unsigned int Socket::getPeerAddress()
{
	return socket->peerAddress().toIPv4Address();
}

unsigned short Socket::getPeerPort()
{
	return socket->peerPort();
}

Socket::~Socket()
{
}
