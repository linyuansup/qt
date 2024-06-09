#include "socket_list.h"

SocketList::SocketList()
{
}

SocketList::~SocketList()
{
}

void SocketList::add(Socket* socket)
{
	mutex.lock();
	vec.push_back(socket);
	mutex.unlock();
}

void SocketList::remove(Socket* socket)
{
	mutex.lock();
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		if (vec[i] == socket)
		{
			vec.erase(vec.begin() + i);
			break;
		}
	}
	mutex.unlock();
}

Socket* SocketList::find(unsigned int address, unsigned short port)
{
	mutex.lock();
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		//if (vec[i]->getPeerAddress() == address && vec[i]->getPeerPort() == port)
		if (vec[i]->getPeerAddress() == address)
		{
			mutex.unlock();
			return vec[i];
		}
	}
	mutex.unlock();
	return nullptr;
}
