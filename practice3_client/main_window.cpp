#include "main_window.h"
#include "./ui_main_window.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->sendButton->setEnabled(false);
    ui->portEdit->setText(QString::number(port));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_connectButton_clicked()
{
    if (!status)
    {
        QString ip = ui->ipEdit->text();
        if (!serverIP->setAddress(ip))
        {
            QMessageBox::information(this, "error", "server ip address error!");
            return;
        }

        if (ui->userEdit->text() == "")
        {
            QMessageBox::information(this, "error", "User name error!");
            return;
        }

        userName = ui->userEdit->text();
        connect(tcpSocket, &QTcpSocket::connected, this, &MainWindow::slotConnected);
        connect(tcpSocket, &QTcpSocket::disconnected, this, &MainWindow::slotDisconnected);
        connect(tcpSocket, &QTcpSocket::readyRead, this, &MainWindow::dataReceived);

        tcpSocket->connectToHost(*serverIP, ui->portEdit->text().toInt());

        status = true;
    }
    else
    {
        QString msg = userName + "离开";
        QByteArray str = msg.toUtf8();
        tcpSocket->write(str, str.length());
        tcpSocket->disconnectFromHost();
        status = false;
    }
}


void MainWindow::on_sendButton_clicked()
{
    if (ui->messageEdit->text() == "")
    {
        return;
    }
    QString msg = userName + ":" + ui->messageEdit->text();
    QByteArray str = msg.toUtf8();
    tcpSocket->write(str, str.length());
    ui->messageEdit->clear();
}

void MainWindow::slotConnected()
{
    ui->sendButton->setEnabled(true);
    ui->connectButton->setText("与服务器连接");

    QString msg = "欢迎" + userName;
    QByteArray str = msg.toUtf8();
    tcpSocket->write(str, str.length());
}

void MainWindow::slotDisconnected()
{
    ui->sendButton->setEnabled(false);
    ui->connectButton->setText("断开与服务器连接");
}

void MainWindow::dataReceived()
{
    while (tcpSocket->bytesAvailable() > 0)
    {
        QByteArray datagram;
        datagram.resize(tcpSocket->bytesAvailable());

        tcpSocket->read(datagram.data(), datagram.size());

        QString msg = datagram.data();
        ui->textArea->setText(ui->textArea->toPlainText() + "\n" + msg.left(datagram.size()));
    }
}


