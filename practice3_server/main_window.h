#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QTcpServer>

#include "server.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
class Server;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateServer(QString, int);
	void on_startServerButton_clicked();

private:
    Ui::MainWindow *ui;
    Server* server;
};
#endif // MAIN_WINDOW_H
