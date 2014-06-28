#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QLabel"
#include "ui_mainwindow.h"
#include <QPixmap>

class qt_thermo_vision_controller;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(char* ptr, QWidget *parent = 0);
	~MainWindow();
	char* data_ptr;

public slots:
	void update(QPixmap image);

signals:
	void exposure_time_changed(int new_exposure_time_ms);
	void gain_changed(int new_gain_percent);

private:
	Ui::MainWindow *ui;

	friend qt_thermo_vision_controller;
};

#endif // MAINWINDOW_H

