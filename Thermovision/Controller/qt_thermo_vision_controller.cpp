#include "qt_thermo_vision_controller.h"
#include "Model/thermo_camera_model.h"
#include "View/initialinfodialog.h"
#include "log.h"
#include <Qdir>
#include <QMessageBox>

qt_thermo_vision_controller::qt_thermo_vision_controller():
	the_model(new thermo_camera_model),
	w(the_model->get_data_pointer())
{
	w.show();

	connect(w.ui->actionCalibration, SIGNAL(triggered()), this, SLOT(calibration_requested()));
}

void qt_thermo_vision_controller::run()
{
	the_model->image_capture();
}

void qt_thermo_vision_controller::calibration_requested()
{
	InitialInfoDialog initial_info_dialog;


	QMessageBox::warning(0, "Communique", "Please, enter the following data as precisely as possible. This will help you to identify the configuration in future");


	log_debug(QDir::currentPath().toStdString().c_str());

	if( initial_info_dialog.exec() == QDialog::Rejected )
		return;

	the_model->run_calibration(initial_info_dialog.get_given_calibration_parameters());
}
