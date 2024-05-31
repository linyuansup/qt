#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

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
    void on_calendar_clicked(const QDate &date);
    void on_newFile_triggered();
    void on_open_triggered();
    void on_redo_triggered();
    void on_undo_triggered();
    void on_cut_triggered();
    void on_copy_triggered();
    void on_paste_triggered();
    void on_save_triggered();
    void on_textEdit_textChanged();
    void on_font_triggered();
    void on_color_triggered();

private:
    bool checkSave() const;
    void initUi();

private:
    Ui::MainWindow *ui;
    bool saved = true;
};
#endif // MAIN_WINDOW_H
