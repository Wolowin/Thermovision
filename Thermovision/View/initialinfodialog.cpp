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
	parameters.camera_model = ui->ComboBox_camera_model->currentText();
	parameters.filter_model = ui->ComboBox_filter_model->currentText();
	parameters.distance_to_obj = ui->SpineBox_distance->value();
	parameters.lens_focal = ui->Spinbox_focal->value();
	parameters.Gain = ui->SpinBox_Gain->value();
	parameters.exposure_time = ui->SpinBox_Exposure->value();

	return parameters;
}
