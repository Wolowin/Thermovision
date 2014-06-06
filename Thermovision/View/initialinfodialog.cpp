#include "initialinfodialog.h"
#include "ui_initialinfodialog.h"

#include <Qdir>
InitialInfoDialog::InitialInfoDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::InitialInfoDialog)
{
	ui->setupUi(this);

//	ui->TODO_remove_label->setText(QDir::currentPath().toStdString().c_str());
}

InitialInfoDialog::~InitialInfoDialog()
{
	delete ui;
}

calibration_parameters InitialInfoDialog::get_given_calibration_parameters()
{
	calibration_parameters parameters;

	parameters.profile_name = ui->Edit_calibrationProfileName->text();
	parameters.camera_model = ui->ComboBox_camera_model->;
	parameters.filter_model = ui->ComboBox_filter_model;
	parameters.distance_to_obj = ui->SpineBox_distance;
	parameters.lens_focal = ui->Spinbox_focal;
	parameters.Gain = ui->SpinBox_Gain;
	parameters.exposure_time = ui->SpinBox_Exposure;

	return calibration_parameters();

}
