#include "main_window.h"
#include "./ui_main_window.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_connectButton_clicked()
{
	socket = client.connectTo(QHostAddress("127.0.0.1").toIPv4Address(), 8888);
}

void MainWindow::on_sendButton_clicked()
{
	//client.sendText(socket, "12345·³·³·³", QHostAddress("127.0.0.1").toIPv4Address(), 8889);
	unsigned short port = ui->targetPortEdit->text().toUShort();
	auto filePath = QFileDialog::getOpenFileName(this, "Open File", "", "All Files (*)");
	if (filePath.isEmpty()) {
		return;
	}
	auto file = new QFile(filePath);
	if (!file->open(QIODevice::ReadOnly)) {
		ui->textArea->append("Failed to open file.");
		return;
	}
	client.sendFile(socket, file, QHostAddress("127.0.0.1").toIPv4Address(), port);
}
