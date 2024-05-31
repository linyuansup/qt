#include "main_window.h"
#include "./ui_main_window.h"

#include <qevent.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    ui->pictureLabel->setGeometry(event->pos().x() - 315/2, event->pos().y() - 315/2, 315,315);
}
