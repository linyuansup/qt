#include "main_window.h"
#include "./ui_main_window.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName("chat.db");
	if (!db.open()) {
		QMessageBox msgBox;
		QSqlError error = db.lastError();
		msgBox.setText("Database error." + error.text());
		msgBox.exec();
	}
	refreshData();
	connect(client, &Client::messageReceived, [this](TextMessage* message) {
		addData(message->headInfo->address.toString(), message->headInfo->port, message->message, false);
		});
	connect(client, &Client::onProcess, [this](ServerHead* server, FileHead* message, unsigned long long readed, unsigned long long total) {
		if (currentUser->ip == server->address.toIPv4Address() && currentUser->port == server->port) {
			ui->progressBar->setValue((int)(readed * 100 / total));
		}
		});
	connect(client, &Client::disconnected, [this] {
		QMessageBox msgBox;
		msgBox.setText("Connect lost.");
		msgBox.exec();
		});
	connect(client, &Client::fileReceived, [this](ServerHead* server, FileHead* message, QByteArray* content) {
		addData(server->address.toString(), server->port, "[File] " + message->filename, false);
		QMessageBox msgBox;
		msgBox.setText("A file has been recieved at download folder");
		msgBox.exec();
		});
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_connectButton_clicked()
{
	QHostAddress address(ui->ipEdit->text());
	socket = client->connectTo(address.toIPv4Address(), ui->portEdit->text().toUShort());
}

void MainWindow::on_sendMessage_clicked()
{
	if (currentUser == nullptr) {
		return;
	}
	addData(QHostAddress(currentUser->ip).toString(), currentUser->port, ui->messageEdit->text(), true);
	client->sendText(socket, ui->messageEdit->text(), currentUser->ip, currentUser->port);
}

void MainWindow::on_sendFile_clicked()
{
	if (currentUser == nullptr) {
		return;
	}
	QFile file(QFileDialog::getOpenFileName(this, "Select file"));
	file.open(QIODevice::ReadOnly);
	auto filename = file.fileName();
	filename = filename.right(filename.size() - filename.lastIndexOf("/") - 1);
	client->sendFile(socket, &file, currentUser->ip, currentUser->port);
	file.close();
	addData(QHostAddress(currentUser->ip).toString(), currentUser->port, "[File] " + filename, true);
}

void MainWindow::on_newConnect_clicked()
{
	auto ip = QInputDialog::getText(this, "New connection", "IP:");
	auto port = QInputDialog::getText(this, "New connection", "Port:");
	addContact(ip, port.toInt());
	currentUser = users[users.size() - 1];
}

void MainWindow::on_contactList_clicked(const QModelIndex& index)
{
	currentUser = users[index.row()];
	refreshDetail();
}

void MainWindow::addData(QString ip, int port, QString message, bool sendByMe)
{
	addContact(ip, port);
	QSqlQuery query;
	query.prepare("INSERT INTO message (ip, port, message, time, me) VALUES (:ip, :port, :message, :time, :me)");
	query.bindValue(":ip", ip);
	query.bindValue(":port", port);
	query.bindValue(":message", message);
	query.bindValue(":time", QDateTime::currentDateTime().toString());
	query.bindValue(":me", sendByMe);
	query.exec();
	refreshData();
}

void MainWindow::addContact(QString ip, int port)
{
	if (findContact(ip, port)) {
		return;
	}
	QSqlQuery query;
	query.prepare("INSERT INTO contact (ip, port) VALUES (:ip, :port)");
	query.bindValue(":ip", ip);
	query.bindValue(":port", port);
	query.exec();
	refreshData();
}

bool MainWindow::findContact(QString ip, int port)
{
	QSqlQuery query;
	query.prepare("SELECT * FROM contact WHERE ip = :ip AND port = :port");
	query.bindValue(":ip", ip);
	query.bindValue(":port", port);
	query.exec();
	return query.next();
}

void MainWindow::refreshData()
{
	QSqlQuery query;
	query.prepare("SELECT * FROM contact");
	query.exec();
	QStringList list;
	users.clear();
	while (query.next()) {
		list << query.value(0).toString() + " " + query.value(1).toString();
		QHostAddress address(query.value(0).toString());
		auto userInfo = new UserInfo();
		userInfo->ip = address.toIPv4Address();
		userInfo->port = query.value(1).toString().toUShort();
		users.push_back(userInfo);
	}
	QStringListModel* model = new QStringListModel(list);
	model->setStringList(list);
	ui->contactList->setModel(model);
	refreshDetail();
}

void MainWindow::refreshDetail()
{
	if (currentUser == nullptr) {
		return;
	}
	QSqlQuery query;
	query.prepare("SELECT * FROM message WHERE (ip = :ip AND port = :port)");
	query.bindValue(":ip", QHostAddress(currentUser->ip).toString());
	query.bindValue(":port", currentUser->port);
	query.exec();
	ui->messageList->clear();
	while (query.next()) {
		if (query.value(4).toInt() == 1) {
			ui->messageList->insertPlainText("You: " + query.value(3).toString() + " " + query.value(2).toString() + "\n");
		}
		else {
			ui->messageList->insertPlainText(query.value(3).toString() + " " + query.value(2).toString() + "\n");
		}
	}
}
