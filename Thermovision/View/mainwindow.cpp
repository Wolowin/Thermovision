#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QTimer"

MainWindow::MainWindow(char *ptr, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	data_ptr(ptr)
{
	ui->setupUi(this);
	ui->groupBox_calibration->setVisible(false);
	ui->Groupbox_measurement->setVisible(false);

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::update(QPixmap image)
{
	ui->imageLabel->setPixmap(image);
}




