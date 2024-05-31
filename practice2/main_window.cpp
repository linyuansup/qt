#include "main_window.h"
#include "./ui_main_window.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	videoDevicesGroup->setExclusive(true);
	updateCameras();
	mediaCaptureSession->setCamera(camera);
	mediaCaptureSession->setVideoOutput(ui->video);
	connect(videoDevicesGroup, &QActionGroup::triggered, this, &MainWindow::updateCameraDevice);
	connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &MainWindow::on_duration_change);
	connect(mediaPlayer, &QMediaPlayer::playbackStateChanged, this, &MainWindow::on_playback_change);
	connect(ui->slider, &QSlider::sliderMoved, this, &MainWindow::on_slider_actionTriggered);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_start_triggered()
{
	camera->start();
}

void MainWindow::on_close_triggered()
{
	camera->stop();
}

void MainWindow::on_open_triggered()
{
	mediaPlayer->setVideoOutput(ui->video);
	mediaPlayer->setAudioOutput(audioOutput);
	mediaPlayer->setSource(QFileDialog::getOpenFileUrl());
	mediaPlayer->play();
}

void MainWindow::on_capture_triggered()
{
	delete imageCapture;
	imageCapture = new QImageCapture();
	mediaCaptureSession->setImageCapture(imageCapture);
	imageCapture->captureToFile(QFileDialog::getSaveFileName(this, "Save Image", "", "Images (*.png *.jpg)"));
}

void MainWindow::on_exit_triggered()
{
	camera->stop();
	this->close();
}

void MainWindow::on_record_triggered()
{
	mediaCaptureSession->setRecorder(mediaRecorder);
	mediaRecorder->setOutputLocation(QFileDialog::getSaveFileName(this, "Save Video", "", "Videos (*.mp4)"));
	mediaRecorder->record();
}

void MainWindow::on_stop_triggered()
{
	mediaRecorder->stop();
}

void MainWindow::updateCameras()
{
	for (QAction* action : videoDevicesGroup->actions()) {
		if (!action) {
			delete action;
		}
	}
	for (QAction* action : ui->menuDevices->actions()) {
		if (!action) {
			delete action;
		}
	}
	ui->menuDevices->clear();
	auto availableCameras = QMediaDevices::videoInputs();
	for (const QCameraDevice& device : availableCameras) {
		auto videoDeviceAction = new QAction(device.description(), (QObject*)videoDevicesGroup);
		videoDeviceAction->setCheckable(true);
		videoDeviceAction->setData(QVariant::fromValue(device));
		if (device == QMediaDevices::defaultVideoInput()) {
			videoDeviceAction->setChecked(true);
		}
		ui->menuDevices->addAction(videoDeviceAction);
	}
}
void MainWindow::updateCameraDevice(QAction* action)
{
	camera->stop();
	delete camera;
	camera = new QCamera(qvariant_cast<QCameraDevice>(action->data()));
	mediaCaptureSession->setCamera(camera);
	camera->start();
}



void MainWindow::on_controlButton_clicked()
{
	if (mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
		mediaPlayer->pause();
	}
	else {
		mediaPlayer->play();
	}
}

void MainWindow::on_slider_actionTriggered(qint64 value)
{
	mediaPlayer->setPosition(value);
}

void MainWindow::on_duration_change(qint64 position)
{
	ui->slider->setMinimum(0);
	ui->slider->setMaximum(mediaPlayer->duration());
	ui->slider->setValue(position);
}

void MainWindow::on_playback_change(QMediaPlayer::PlaybackState newState)
{
	if (newState == QMediaPlayer::PlayingState) {
        ui->controlButton->setText("暂停");
	}
	else {
        ui->controlButton->setText("播放");
	}
}

