#include "byte_array.h"

ByteArray::ByteArray(QByteArray* data) : data(data)
{
}

ByteArray::~ByteArray()
{
}

void ByteArray::append(QByteArray app)
{
	data->append(app);
}

void ByteArray::append(ByteArray app)
{
	data->append(*(app.data));
}

char ByteArray::as8Bits()
{
	if (data->size() < 1)
	{
		throw "ByteArray::as8Bits() - data size is less than 1";
	}
	return data->at(0);
}

unsigned int ByteArray::as32Bits()
{
	if (data->size() < 4)
	{
		throw "ByteArray::as32Bits() - data size is less than 4";
	}
	unsigned int result = 0;
	for (int i = 3; i >= 0; i--)
	{
		result |= ((data->at(i) & 0xFF) << (i * 8));
	}
	return result;
}

unsigned short ByteArray::as16Bits()
{
	if (data->size() < 2)
	{
		throw "ByteArray::as16Bits() - data size is less than 2";
	}
	unsigned short result = 0;
	for (int i = 1; i >= 0; i--)
	{
		result |= ((data->at(i) & 0xFF) << (i * 8));
	}
	return result;
}

unsigned long long ByteArray::as64Bits()
{
	if (data->size() < 8)
	{
		throw "ByteArray::as64Bits() - data size is less than 8";
	}
	unsigned long long result = 0;
	for (int i = 7; i >= 0; i--)
	{
		result |= ((data->at(i) & 0xFF) << (i * 8));
	}
	return result;
}

QString ByteArray::asString()
{
	return QString::fromUtf8(data->data(), data->size());
}

unsigned long long ByteArray::size()
{
	return data->size();
}
