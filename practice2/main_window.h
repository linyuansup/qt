#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QMediaDevices>
#include <memory>
#include <QCameraDevice>
#include <QCamera>
#include <QGraphicsView>
#include <qfiledialog.h>
#include <QActionGroup>
#include <QMediaCaptureSession>
#include <QImageCapture>
#include <QMediaPlayer>
#include <QMediaRecorder>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>
#include <QAudioOutput>

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

class QCamera;
class QActionGroup;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private slots:
	void on_start_triggered();
	void on_close_triggered();
	void on_open_triggered();
	void on_capture_triggered();
	void on_exit_triggered();
	void on_record_triggered();
	void on_stop_triggered();
	void updateCameras();
	void updateCameraDevice(QAction* action);
	void on_controlButton_clicked();
	void on_slider_actionTriggered(qint64 value);
	void on_duration_change(qint64 position);
	void on_playback_change(QMediaPlayer::PlaybackState newState);

private:
	Ui::MainWindow* ui;
	QActionGroup* videoDevicesGroup = new QActionGroup(this);
	QCamera* camera = new QCamera(QMediaDevices::defaultVideoInput());
	QMediaCaptureSession* mediaCaptureSession = new QMediaCaptureSession();
	QImageCapture* imageCapture = new QImageCapture();
	QMediaRecorder* mediaRecorder = new QMediaRecorder();
	QMediaPlayer* mediaPlayer = new QMediaPlayer(this);
	QVideoWidget* videoWidget = new QVideoWidget();
	QAudioOutput* audioOutput = new QAudioOutput();
};
#endif // MAIN_WINDOW_H
