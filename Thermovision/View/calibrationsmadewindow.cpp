#include "calibrationsmadewindow.h"
#include "ui_calibrationsmadewindow.h"
#include <QtGui>
#include "log.h"

CalibrationsMadeWindow::CalibrationsMadeWindow(gain_exposure_temp_map the_map, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CalibrationsMadeWindow),
	copied_map(the_map)
{
	log_debug("q");
	ui->setupUi(this);
	QStandardItemModel *model = new QStandardItemModel;

	ga_ex_te_map_iterator it;
	for (it = copied_map.begin() ; it != copied_map.end() ; it ++)
	{

		QStandardItem *gain = new QStandardItem(it->first);
		exposure_temp_map exp_temp_map = it->second;
		ex_te_map_iterator it2;
		for (it2 = exp_temp_map.begin() ; it2 != exp_temp_map.end() ; it2 ++)
		{
			QStandardItem *exposure = new QStandardItem(it2->first);
			std::vector<QString> temp_vec = it2->second;
			std::vector<QString>::iterator it3;

			for (it3 = temp_vec.begin() ; it3 != temp_vec.end() ; it3 ++)
			{
				QStandardItem *temp = new QStandardItem(*it3);
				exposure->appendRow(temp);
			}
			gain->appendRow(exposure);
		}
		model->appendRow(gain);
	}
	ui->columnView->setModel(model);
}

CalibrationsMadeWindow::~CalibrationsMadeWindow()
{
	delete ui;
}
