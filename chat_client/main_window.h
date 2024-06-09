#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QHostAddress>
#include <QFile>

#include "socket.h"
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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Client client;
    Socket* socket;

private slots:
    void on_connectButton_clicked();
    void on_sendButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAIN_WINDOW_H
