#include "./ui_main_window.h"
#include "./main_window.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	initUi();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::initUi() {
	QStringList csList, seList;
	csList << "计算机科学与技术" << "智能科学与技术" << "信息与通信工程";
	seList << "软件工程";
	academyCourseList.insert(QString("计算机"), csList);
	academyCourseList.insert(QString("软件"), seList);
	foreach(auto & academyName, academyCourseList.keys()) {
		ui->academyEdit->addItem(academyName);
	}
}

void MainWindow::on_idEdit_textChanged(const QString& input)
{
	if (input.size() > 10) {
		char fixed[11] = {};
		for (auto i = 0; i < 10; i++) {
			fixed[i] = input[i].unicode();
		}
		fixed[10] = '\0';
		ui->idEdit->setText(QString(fixed));
	}
	if (!input.back().isNumber()) {
		std::shared_ptr<QChar> fixed(new QChar[input.size()]);
		for (auto i = 0; i < input.size(); i++) {
			fixed.get()[i] = input[i];
		}
		ui->idEdit->setText(QString(fixed.get()));
	}
}


void MainWindow::on_ageEdit_valueChanged(int value)
{
	if (value < 0) {
		ui->ageEdit->setValue(0);
	}
	else if (value > 120) {
		ui->ageEdit->setValue(120);
	}
}


void MainWindow::on_academyEdit_currentTextChanged(const QString& text)
{
	auto selected = academyCourseList.find(text);
	if (selected == academyCourseList.end()) {
		return;
	}
	auto& value = selected.value();
	ui->courseEdit->clear();
	foreach(auto & courseName, value) {
		ui->courseEdit->addItem(courseName);
	}
}

void MainWindow::on_pictureButton_clicked()
{
    auto fileName = QFileDialog::getOpenFileName(this, "打开图片", QDir::homePath(), "*.jpg\n*.bmp\n*.png");
    if (fileName.isEmpty()) {
        return;
    }
    ui->pictureLabel->setPixmap(QPixmap(fileName));
}

