#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QTimer"

MainWindow::MainWindow(char *ptr, QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	data_ptr(ptr)
{
	ui->setupUi(this);

	qtimer_workaround(); //TODO
}

void MainWindow::qtimer_workaround()
{
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(10);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::update()
{
	uchar* uptr=(uchar*)data_ptr;
	QImage myImage(uptr, 768, 576, QImage::Format_RGB32 );
	ui->imageLabel->setPixmap(QPixmap::fromImage(myImage));
}




