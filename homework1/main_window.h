#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <memory>
#include <QFileDialog>

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

private:
    Ui::MainWindow* ui;
    QMap<QString, QStringList> academyCourseList;

private slots:
    void on_idEdit_textChanged(const QString &input);
    void on_ageEdit_valueChanged(int value);

    void on_academyEdit_currentTextChanged(const QString &text);

    void on_pictureButton_clicked();

private:
    void initUi();
};
#endif // MAIN_WINDOW_H
