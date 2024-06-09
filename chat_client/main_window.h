#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QHostAddress>
#include <QSqlDatabase>
#include <QInputDialog>
#include <QStringListModel>
#include <QDateTime>
#include <QSqlQuery>
#include <QFile>
#include <QSqlError>
#include <QModelIndex>

#include "socket.h"
#include "user_info.h"
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private slots:
	void on_connectButton_clicked();
	void on_sendMessage_clicked();
	void on_sendFile_clicked();
	void on_newConnect_clicked();
	void on_contactList_clicked(const QModelIndex& index);

private:
	void addData(QString ip, int port, QString message, bool sendByMe);
	void addContact(QString ip, int port);
	bool findContact(QString ip, int port);
	void refreshData();
	void refreshDetail();

private:
	Ui::MainWindow* ui;
	Client* client = new Client();
	UserInfo* currentUser = nullptr;
	std::vector<UserInfo*> users;
	Socket* socket;
};
#endif // MAIN_WINDOW_H
