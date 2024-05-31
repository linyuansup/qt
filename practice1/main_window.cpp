#include "main_window.h"
#include "./ui_main_window.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	initUi();
}

MainWindow::~MainWindow()
{
	db.close();
	delete ui;
}

void MainWindow::initUi()
{
	QStringList headers;
	headers << "学号" << "姓名" << "课程" << "成绩";
	ui->tableWidget->setHorizontalHeaderLabels(headers);
	ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

bool MainWindow::checkSave() const
{
	if (!isSaved) {
		QMessageBox msg;
		msg.setInformativeText("当前数据库没有保存，继续吗？");
		msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
		if (msg.exec() != QMessageBox::Ok) {
			return false;
		}
	}
	return true;
}

void MainWindow::on_add_triggered()
{
	auto selectedItems = ui->tableWidget->selectedRanges();
	int bottomIndex = -1;
	for (auto itemItr = selectedItems.begin(); itemItr != selectedItems.end(); itemItr++) {
		bottomIndex = std::max(itemItr->bottomRow() + 1, bottomIndex);
	}
	if (bottomIndex == -1) {
		bottomIndex = ui->tableWidget->rowCount();
	}
	ui->tableWidget->insertRow(bottomIndex);
}

void MainWindow::on_remove_triggered()
{
	auto selectedItems = ui->tableWidget->selectedRanges();
	std::vector<int> selectedRow;
	for (auto itemItr = selectedItems.begin(); itemItr != selectedItems.end(); itemItr++) {
		for (int i = itemItr->topRow(); i <= itemItr->bottomRow(); i++) {
			selectedRow.push_back(i);
		}
	}
	std::sort(selectedRow.begin(), selectedRow.end());
	for (auto itemItr = selectedRow.rbegin(); itemItr < selectedRow.rend(); itemItr++) {
		ui->tableWidget->removeRow(*itemItr);
	}
}

void MainWindow::on_open_triggered()
{
    if (!checkSave()) {
		return;
	}
	if (db.isOpen()) {
		db.close();
	}
	auto path = QFileDialog::getOpenFileName(this, "打开", QDir::homePath(), "*.db");
	db.setDatabaseName(path);
	if (!db.open()) {
		QMessageBox::critical(this, "错误", "数据库打开失败", QMessageBox::Yes);
		return;
	}
	QSqlQuery query;
	bool flg = query.exec("SELECT id, name, course, score FROM student");
	if (flg) {
		while (query.next()) {
			ui->tableWidget->insertRow(ui->tableWidget->rowCount());
			ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 0, new QTableWidgetItem(query.value("id").toString()));
			ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 1, new QTableWidgetItem(query.value("name").toString()));
			ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 2, new QTableWidgetItem(query.value("course").toString()));
			ui->tableWidget->setItem(ui->tableWidget->rowCount() - 1, 3, new QTableWidgetItem(query.value("score").toString()));
		}
	}
	else {
		QMessageBox::critical(this, "错误", "数据库结构错误", QMessageBox::Yes);
		return;
	}
}

void MainWindow::on_tableWidget_cellChanged(int row, int column)
{
	isSaved = false;
}


void MainWindow::on_exit_triggered()
{
	if (!checkSave()) {
		return;
	}
	close();
}


void MainWindow::on_save_triggered()
{
    if (!db.isOpen()) {
		QMessageBox::critical(this, "错误", "数据库未打开", QMessageBox::Yes);
		return;
	}
	{
		QSqlQuery query;
		bool flg = query.exec("DELETE FROM student");
		if (!flg) {
			QMessageBox::critical(this, "错误", "操作失败", QMessageBox::Yes);
			return;
		}
	}
	QSqlQuery query;
	query.prepare("INSERT INTO student VALUES (:id, :name, :course, :score)");
	QList<int> errorRows;
	for (int i = 0; i < ui->tableWidget->rowCount(); i++) {
		int id = ui->tableWidget->item(i, 0)->text().toInt();
		QString name = ui->tableWidget->item(i, 1)->text();
		QString course = ui->tableWidget->item(i, 2)->text();
		int score = ui->tableWidget->item(i, 3)->text().toInt();
		query.bindValue(0, id);
		query.bindValue(1, name);
		query.bindValue(2, course);
		query.bindValue(3, score);
		bool flag = query.exec();
		if (!flag) {
			errorRows << i + 1;
		}
	}
	if (errorRows.isEmpty()) {
		isSaved = true;
		QMessageBox::information(this, "提示", "保存成功");
	}
	else {
		QString errMessage = "以下列保存失败：\n";
		for (auto iter = errorRows.begin(); iter != errorRows.end(); iter++) {
			errMessage += *iter + "\n";
		}
		QMessageBox::warning(this, "警告", errMessage);
	}
}

