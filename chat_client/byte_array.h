#ifndef BYTE_ARRAY_H
#define BYTE_ARRAY_H

#include <QByteArray>
#include <QString>
#include <QObject>

class ByteArray : public QObject {

	Q_OBJECT
public:
	QByteArray* data = nullptr;

public:
	ByteArray(QByteArray* data);
	~ByteArray();

	void append(QByteArray data);
	void append(ByteArray data);
	char as8Bits();
	unsigned int as32Bits();
	unsigned short as16Bits();
	unsigned long long as64Bits();
	QString asString();
	unsigned long long size();
};

#endif // BYTE_ARRAY_H
