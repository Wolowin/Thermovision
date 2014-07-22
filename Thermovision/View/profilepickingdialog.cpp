#include "profilepickingdialog.h"
#include "ui_profilepickingdialog.h"

#include "Model/xml_handler.h"
#include "log.h"

ProfilePickingDialog::ProfilePickingDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::ProfilePickingDialog)
{
	ui->setupUi(this);
	XML_handler the_xml_handler;
	parameters_vector = the_xml_handler.get_profiles_with_parameters();
	ui->profiles_table->setRowCount(parameters_vector.size());
	for (int row = 0 ; row < parameters_vector.size() ; row ++)
	{
		QTableWidgetItem *profile_item = new QTableWidgetItem(parameters_vector[row].profile_name);
		ui->profiles_table->setItem(row, 0, profile_item);
		QTableWidgetItem *camera_model_item = new QTableWidgetItem(parameters_vector[row].camera_model);
		ui->profiles_table->setItem(row, 1, camera_model_item);
		QTableWidgetItem *filter_model_item = new QTableWidgetItem(parameters_vector[row].filter_model);
		ui->profiles_table->setItem(row, 2, filter_model_item);
		QTableWidgetItem *distance_item = new QTableWidgetItem(QString::number(parameters_vector[row].distance_to_obj));
		ui->profiles_table->setItem(row, 3, distance_item);
		QTableWidgetItem *lens_item = new QTableWidgetItem(QString::number(parameters_vector[row].lens_focal));
		ui->profiles_table->setItem(row, 4, lens_item);
		QTableWidgetItem *gain_item = new QTableWidgetItem(QString::number(parameters_vector[row].Gain));
		ui->profiles_table->setItem(row, 5, gain_item);
		QTableWidgetItem *exposure_item = new QTableWidgetItem(QString::number(parameters_vector[row].exposure_time));
		ui->profiles_table->setItem(row, 6, exposure_item);
	}

	ui->profiles_table->setSelectionBehavior( QAbstractItemView::SelectRows );
	ui->profiles_table->setSelectionMode( QAbstractItemView::SingleSelection );
}

ProfilePickingDialog::~ProfilePickingDialog()
{
	delete ui;
}

calibration_parameters ProfilePickingDialog::get_picked_profile_parameters()
{
	int row = ui->profiles_table->currentRow();
	if ( ui->profiles_table->selectedItems().count() == 0 )
	{
		log_debug("No item sellected");
		calibration_parameters empty_return;
		empty_return.profile_name = QString();
		return empty_return;
	}
	cout << "current row " << row << endl;
	return parameters_vector[row];
}
