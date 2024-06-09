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

void MainWindow::on_startServerButton_clicked()
{
	server = new Server(ui->portEdit->text().toInt());
    connect(server, &Server::updateServer, this, &MainWindow::updateServer);
    ui->startServerButton->setEnabled(false);
}

void MainWindow::updateServer(QString msg, int length)
{
    QByteArray str = msg.toUtf8();
	ui->textArea->setText(ui->textArea->toPlainText() + "\n" + str.left(length));
}

