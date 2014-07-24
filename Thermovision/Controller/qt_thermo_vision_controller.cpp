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

	connect(main_window.ui->actionCalibration, SIGNAL(triggered()), this, SLOT(show_calibration_toolbar()));
	connect(main_window.ui->actionMeasurement, SIGNAL(triggered()), this, SLOT(start_measurement()));
	connect(main_window.ui->actionStop_Measurement, SIGNAL(triggered()), this, SLOT(stop_measurement()));
	connect(the_model.get(), SIGNAL(picture_changed(QPixmap)), &main_window, SLOT(update(QPixmap)));
	connect(main_window.ui->comboBox_profile_name, SIGNAL(currentIndexChanged (QString)), this, SLOT(show_profile_details(QString )));
	connect(main_window.ui->button_create_new_profile, SIGNAL(clicked()),this, SLOT(create_new_profile()));
	connect(main_window.ui->button_CapturePhoto, SIGNAL(clicked()),this, SLOT(calibration_photo_capture_initialized()));

	connect(the_model.get(), SIGNAL(change_view_gain_value(int)), main_window.ui->spinBox_Gain, SLOT(setValue(int)));
	connect(the_model.get(), SIGNAL(change_view_exposure_value(int)), main_window.ui->spinBox_exposureTime, SLOT(setValue(int)));

	connect(main_window.ui->spinBox_Gain, SIGNAL(valueChanged(int)),the_model.get() , SLOT(gain_changed_by_user(int)));
	connect(main_window.ui->spinBox_exposureTime, SIGNAL(valueChanged(int)), the_model.get(), SLOT(exposure_changed_by_user(int)));
	connect(main_window.ui->doubleSpinBox_emissivity, SIGNAL(valueChanged(double)), the_model.get(), SLOT (emissivity_changed_by_user(double)));

	the_model->post_slot_connection_initialization();


	update_profiles_list();
	hide_profile_details_labels();
}

void qt_thermo_vision_controller::hide_profile_details_labels()
{
	main_window.ui->comboBox_profile_name->setCurrentIndex(-1);
	change_profile_details_visibility(false);
}

void qt_thermo_vision_controller::show_calibration_toolbar()
{
	main_window.ui->groupBox_calibration->setVisible(true);
	main_window.ui->Groupbox_measurement->setVisible(false);

	hide_profile_details_labels();
	the_model->start_calibration_video();
}

void qt_thermo_vision_controller::start_measurement()
{


	ProfilePickingDialog profile_picking_dialog;
	QString picked_profile = QString();
	calibration_parameters profile_parameters;
	while (picked_profile == QString())
	{
		if( profile_picking_dialog.exec() == QDialog::Rejected )
		{
			return;
		}
		profile_parameters = profile_picking_dialog.get_picked_profile_parameters();
		picked_profile = profile_parameters.profile_name;
		if (picked_profile == QString())
		{
			QMessageBox::warning(0, "Communique", "You need to pick a profile to continue");
		}
	}

	main_window.ui->groupBox_calibration->setVisible(false);
	main_window.ui->Groupbox_measurement->setVisible(true);
	show_legend_bar_label();

	the_model->gain_changed_by_user(profile_parameters.Gain);
	the_model->exposure_changed_by_user(profile_parameters.exposure_time);

	LUT_table the_lut_table = XML_handler().get_profile_LUT_table(profile_parameters);

	the_model->run_measurement(the_lut_table);
}

void qt_thermo_vision_controller::stop_measurement()
{
	the_model->end_measurement();
	main_window.ui->legend_bar_label->setVisible(false);
}

void qt_thermo_vision_controller::show_profile_details(QString profile_name)
{
	XML_handler tmp_xml_handler;
	calibration_parameters tmp_calibration_parameters = tmp_xml_handler.get_profile_details(profile_name);

	main_window.ui->label_camera_model->setText(tmp_calibration_parameters.camera_model);
	main_window.ui->label_filter_model->setText(tmp_calibration_parameters.filter_model);
	main_window.ui->label_lens_focal->setText(QString::number(tmp_calibration_parameters.lens_focal));
	main_window.ui->label_distance_from_object->setText(QString::number(tmp_calibration_parameters.distance_to_obj));

	change_profile_details_visibility(true);
}

void qt_thermo_vision_controller::create_new_profile()
{
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
		the_model->create_new_profile(received_calib_params);
		update_profiles_list();
}

void qt_thermo_vision_controller::calibration_photo_capture_initialized()
{
	XML_handler tmp_xml_handler;
	cout << " main_window.ui->comboBox_profile_name->currentText(): " << main_window.ui->comboBox_profile_name->currentText().toStdString() << endl;
	calibration_parameters tmp_calibration_parameters = tmp_xml_handler.get_profile_details(main_window.ui->comboBox_profile_name->currentText());
	if (tmp_calibration_parameters.profile_name == QString())
	{
		QMessageBox::warning(0, "Communique", "Must pick a profile");
		return;
	}

	tmp_calibration_parameters.exposure_time = main_window.ui->spinBox_exposureTime->value();
	tmp_calibration_parameters.Gain = main_window.ui->spinBox_Gain->value();

	the_model->run_calibration(tmp_calibration_parameters);
}

void qt_thermo_vision_controller::update_profiles_list()
{
	cout <<  main_window.ui->comboBox_profile_name->count() << endl;
	int count = main_window.ui->comboBox_profile_name->count();
	for (int i = 0 ; i < count ; i ++)
	{
		main_window.ui->comboBox_profile_name->removeItem(0);
		cout << "r" << endl;
	}
	XML_handler temp_xml_handler;
	cout << main_window.ui->comboBox_profile_name->count() << endl;
	cout << "qweqwe" << temp_xml_handler.get_profiles_names().size() << endl;
	main_window.ui->comboBox_profile_name->addItems(temp_xml_handler.get_profiles_names());
	hide_profile_details_labels();
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

void qt_thermo_vision_controller::change_profile_details_visibility(bool show)
{
	main_window.ui->label_camera_model_title->setVisible(show);
	main_window.ui->label_camera_model->setVisible(show);
	main_window.ui->label_filter_model_title->setVisible(show);
	main_window.ui->label_filter_model->setVisible(show);
	main_window.ui->label_lens_focal_title->setVisible(show);
	main_window.ui->label_lens_focal->setVisible(show);
	main_window.ui->label_distance_from_object_title->setVisible(show);
	main_window.ui->label_distance_from_object->setVisible(show);
}
