#include "main_window.h"
#include "./ui_main_window.h"

#include <qdatetime.h>
#include <qfiledialog.h>
#include <qfile.h>
#include <qfontdialog.h>
#include <qmessagebox.h>
#include <qcolordialog.h>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	initUi();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_calendar_clicked(const QDate &date)
{
	ui->textEdit->setText(ui->textEdit->toPlainText()
		+ date.toString() + "\n");
	saved = false;
}

void MainWindow::on_newFile_triggered()
{
	if (!checkSave())
	{
		return;
	}
	ui->textEdit->setText("");
}

bool MainWindow::checkSave() const
{
	if (!saved)
	{
		QMessageBox msg;
        msg.setInformativeText("当前文件未保存，仍然继续吗？");
		msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
		if (msg.exec() != QMessageBox::Ok)
		{
			return false;
		}
	}
	return true;
}

void MainWindow::initUi()
{
	ui->textEdit->setUndoRedoEnabled(true);
	ui->textEdit->setShortcutEnabled(true);
	ui->textEdit->setUpdatesEnabled(true);
}

void MainWindow::on_open_triggered()
{
	if (!checkSave())
	{
		return;
	}
	auto fileName = QFileDialog::getOpenFileName(this,
		"打开", QDir::homePath(), "*.txt");
	if (fileName.isEmpty())
	{
		return;
	}
	QFile file(fileName);
	file.open(QFile::OpenModeFlag::ReadWrite);
	auto content = file.readAll();
	file.close();
	ui->textEdit->setText(QString(content));
}

void MainWindow::on_redo_triggered()
{
	ui->textEdit->redo();
}

void MainWindow::on_undo_triggered()
{
	ui->textEdit->undo();
}

void MainWindow::on_cut_triggered()
{
	ui->textEdit->cut();
}

void MainWindow::on_copy_triggered()
{
	ui->textEdit->copy();
}

void MainWindow::on_paste_triggered()
{
	ui->textEdit->paste();
}

void MainWindow::on_save_triggered()
{
	auto fileName = QFileDialog::getSaveFileName(this,
		"保存", "", "*.txt");
	if (fileName.isEmpty())
	{
		return;
	}
	auto file = QFile(fileName);
    file.open(QFile::OpenModeFlag::ReadWrite);
	file.write(ui->textEdit->toPlainText()
		.toStdString().c_str());
	file.close();
	saved = true;
}

void MainWindow::on_textEdit_textChanged()
{
	saved = false;
}


void MainWindow::on_font_triggered()
{
	bool ok = false;
	QFont font = QFontDialog::getFont(&ok, this);
	if (ok) {
		ui->textEdit->setFont(font);
	}
}

void MainWindow::on_color_triggered()
{
	QColor color = QColorDialog::getColor(Qt::black, this, "颜色对话框");
	ui->textEdit->setTextColor(color);
}

