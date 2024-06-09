#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QHostAddress>
#include <QTcpSocket>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_connectButton_clicked();
    void on_sendButton_clicked();
    void slotConnected();
    void slotDisconnected();
    void dataReceived();

private:
    Ui::MainWindow *ui;
    bool status = false;
    int port = 8888;
    QHostAddress* serverIP = new QHostAddress();
    QString userName;
    QTcpSocket* tcpSocket = new QTcpSocket(this);
};
#endif // MAIN_WINDOW_H
