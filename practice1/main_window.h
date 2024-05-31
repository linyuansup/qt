#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QtSql/qsqldatabase.h>
#include <QtSql/qsqlerror.h>
#include <QtSql/qsqlquery.h>
#include <qmessagebox.h>
#include <qfiledialog.h>

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
    void on_add_triggered();
    void on_remove_triggered();
    void on_open_triggered();
    void on_tableWidget_cellChanged(int row, int column);
    void on_exit_triggered();
    void on_save_triggered();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    bool isSaved = true;

private:
    void initUi();
    bool checkSave() const;
};
#endif // MAIN_WINDOW_H
