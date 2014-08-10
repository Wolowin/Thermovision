#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QTimer"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
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
	int w = ui->imageLabel->width();
	int h = ui->imageLabel->height();

	// set a scaled pixmap to a w x h window keeping its aspect ratio
	ui->imageLabel->setPixmap(image.scaled(w,h,Qt::KeepAspectRatio));
}

void MainWindow::gain_changed_by_model(int new_gain)
{
	ui->spinBox_Gain->setValue(new_gain);
}

void MainWindow::exposure_changed_by_model(int new_exposure)
{
	ui->spinBox_exposureTime->setValue(new_exposure);
}




