#ifndef TEXT_MESSAGE_H
#define TEXT_MESSAGE_H

#include <QHostAddress>

#include "server_head.h"

class TextMessage : public QObject {
	Q_OBJECT
public:
	ServerHead *headInfo = nullptr;
	QString message;
};

#endif // TEXT_MESSAGE_H
