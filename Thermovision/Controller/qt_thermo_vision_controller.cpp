#include "qt_thermo_vision_controller.h"
#include "Model/thermo_camera_model.h"
#include "View/initialinfodialog.h"
#include "View/profilepickingdialog.h"
#include "log.h"
#include <Qdir>
#include <QMessageBox>

#include "Interface/calibration_parameters.h"
#include "model/xml_handler.h"
#include "Interface/lut_table.h"

qt_thermo_vision_controller::qt_thermo_vision_controller():
	the_model(new thermo_camera_model),
	main_window(the_model->get_data_pointer())
{
	main_window.show();

	connect(main_window.ui->actionCalibration, SIGNAL(triggered()), this, SLOT(calibration_requested()));
	connect(main_window.ui->actionMeasurement, SIGNAL(triggered()), this, SLOT(start_measurement()));
	connect(main_window.ui->actionStop_Measurement, SIGNAL(triggered()), this, SLOT(stop_measurement()));
	connect(the_model.get(), SIGNAL(picture_changed(QPixmap)), &main_window, SLOT(update(QPixmap)));

//	connect(main_window.ui->, SIGNAL(picture_changed(int)), the_model.get(), SLOT(react_to_changed_exposure_time(int)));
//	connect(the_model.get(), SIGNAL(picture_changed(int)), the_model.get(), SLOT(react_to_changed_gain(int)));

}

void qt_thermo_vision_controller::run()
{
	the_model->image_capture();
}

void qt_thermo_vision_controller::calibration_requested()
{
	main_window.ui->groupBox_calibration->setVisible(true);
	main_window.ui->Groupbox_measurement->setVisible(false);
	InitialInfoDialog initial_info_dialog;

	QMessageBox::warning(0, "Communique", "Please, enter the following data as precisely as possible. This will help you to identify the configuration in future");

	calibration_parameters received_calib_params;
	while (received_calib_params.profile_name == QString())
	{
	log_debug(QDir::currentPath().toStdString().c_str());

	if( initial_info_dialog.exec() == QDialog::Rejected )
		return;

	received_calib_params = initial_info_dialog.get_given_calibration_parameters();
	if (received_calib_params.profile_name == QString())
		QMessageBox::warning(0, "Communique", "Profile name is required!");

	}
	the_model->run_calibration(received_calib_params);
}

void qt_thermo_vision_controller::start_measurement()
{
	main_window.ui->groupBox_calibration->setVisible(false);
	main_window.ui->Groupbox_measurement->setVisible(true);

	ProfilePickingDialog profile_picking_dialog;
	QString picked_profile = QString();
	while (picked_profile == QString())
	{
		if( profile_picking_dialog.exec() == QDialog::Rejected )
		{
			return;
		}
		picked_profile = profile_picking_dialog.get_picked_profile();
		if (picked_profile == QString())
		{
			QMessageBox::warning(0, "Communique", "You need to pick a profile to continue");
		}
	}

	show_legend_bar_label();
	cout << picked_profile.toStdString() << endl;

	LUT_table the_lut_table = XML_handler().get_profile_LUT_table(picked_profile);

	the_model->run_measurement(the_lut_table);
//	received_calib_params = initial_info_dialog.get_given_calibration_parameters();
//	if (received_calib_params.profile_name == QString())
//		QMessageBox::warning(0, "Communique", "Profile name is required!");

//	}
	//	the_model->run_calibration(received_calib_params);
}

void qt_thermo_vision_controller::stop_measurement()
{
	the_model->end_measurement();
	main_window.ui->legend_bar_label->setVisible(false);
}

void qt_thermo_vision_controller::show_legend_bar_label()
{
	QImage legend_Bar (30, 256, QImage::Format_Indexed8);

	for (int i = 0 ; i < 256 ; i ++)
	{
		uchar *image_ptr = legend_Bar.scanLine(i);
		for (int j = 0 ; j < 30 ; j ++)
		{
			*image_ptr = 255-i;
			image_ptr++;
		}
	}

	legend_Bar.setColorTable(LUT_table::false_color_qcolortable);

	main_window.ui->legend_bar_label->setPixmap(QPixmap::fromImage(legend_Bar));
	main_window.ui->legend_bar_label->setVisible(true);
}
