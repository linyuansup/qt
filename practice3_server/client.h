#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>
#include <QObject>

class Client : public QTcpSocket
{
    Q_OBJECT
public:
    explicit Client(QObject* parent = nullptr);
signals:
    void updateClients(QString, int);
    void disconnected(int);
protected slots:
    void dataReceived();
    void slotDisconnected();
};

#endif // CLIENT_H
// Path: client.cpp