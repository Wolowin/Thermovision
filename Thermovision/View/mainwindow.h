#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QLabel"
#include "ui_mainwindow.h"

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
	void qtimer_workaround();
public slots:
	void update();

private:
	Ui::MainWindow *ui;

	friend qt_thermo_vision_controller;
};

#endif // MAINWINDOW_H
