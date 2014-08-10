#include "thermo_camera_model.h"
#include "../log.h"

#include <QMessageBox>
#include "Interface/Camera/camera_factory.h"
#include <QFile>

#include <sstream>

#include "abstract_xml_handler.h"
#include "xml_handler.h"
#include <map>

#include "windows.h"

#include "Model/neural_network_wrapper.h"


//TODO REMOVE
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

typedef unsigned long long timestamp_t;

	static timestamp_t
	get_timestamp ()
	{
	  struct timeval now;
	  gettimeofday (&now, NULL);
	  return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
	}
static int for_cout_generation = 0;

thermo_camera_model::thermo_camera_model():
	camera_object(camera_factory::get_camera_object()),
	emissivity(1),
	is_lut_up_to_date(false)
{
	camera_object->initialize_camera();
	image_y = camera_object->get_image_size_y();
	image_x = camera_object->get_image_size_x();
	current_temperature_2dvector.resize(image_y);
	for (int i = 0 ; i < image_y ; i ++)
	{
		current_temperature_2dvector[i].resize(image_x);
	}
	cout << "image_y: " << image_y<< endl;
	cout << "image_x: " << image_x<< endl;
	indexed_image_to_emit =QImage(image_x, image_y, QImage::Format_Indexed8);
	indexed_image_to_emit.setColorTable(LUT_table::false_color_qcolortable);
}
thermo_camera_model::~thermo_camera_model()
{
	camera_object->deinitialize_camera();
}


unsigned char *thermo_camera_model::get_data_pointer()
{
	return reinterpret_cast<unsigned char*> (camera_object->get_data_pointer());
}

void thermo_camera_model::create_new_profile(calibration_parameters the_parameters)
{
	the_calibration_parameters = the_parameters;
	boost::scoped_ptr<abstract_XML_handler> the_xml_handler(new XML_handler);

	if (the_xml_handler->profile_exists(the_calibration_parameters))
	{
		QMessageBox::warning(0, "Profile already exists!", "Profile already exists!");
		return;
	}

	the_xml_handler->add_profile(the_calibration_parameters);
}

void thermo_camera_model::run_measurement(neural_network_wrapper* tmp_neural_network)
{
	log_debug("Starting measurement");

	disconnect(&timer, SIGNAL(timeout()), this, SLOT(emit_calibration_picture()));
	connect(&timer, SIGNAL(timeout()), this, SLOT(emit_measurement_picture()));

	camera_object->start_live_video();
	the_neural_network = tmp_neural_network;
	qtimer_workaround();
}

void thermo_camera_model::end_measurement()
{
	camera_object->stop_live_video();
	timer.stop();
}

void thermo_camera_model::run_calibration(calibration_parameters the_calibration_parameters)
{
	QImage calibration_picture = get_current_picture();

	calibration_picture_dialog.set_image(calibration_picture);

	if( calibration_picture_dialog.exec() == QDialog::Rejected )
	{
		return;
	}

	//saving the picture
//	QString name = QString::number(number_of_picture++);
//	name.append(".jpg");
//	calibration_picture.save(name);
	QImage AOI_image_part = calibration_picture_dialog.get_selected_image_part();
	int avarage_image_value = get_avarage_image_calibration_value(AOI_image_part);
	int temperature = calibration_picture_dialog.get_current_temperature();
	XML_handler tmp_xml_handler;

	tmp_xml_handler.add_calibration_outcome(
				the_calibration_parameters, temperature, avarage_image_value);
	cout << "qwe5"<< endl;
}

void thermo_camera_model::start_calibration_video()
{
	disconnect(&timer, SIGNAL(timeout()), this, SLOT(emit_measurement_picture()));
	connect(&timer, SIGNAL(timeout()), this, SLOT(emit_calibration_picture()));
	camera_object->start_live_video();
	qtimer_workaround();
}

void thermo_camera_model::exposure_changed_by_user(int new_exposure)
{

	int old_exposure;
	int return_status = camera_object->get_exposure_time_in_ms(old_exposure);

	if (new_exposure == old_exposure)
	{
		return;
	}

	if (return_status == -1)
	{
		log_debug("Exposure time not supported");
		emit change_view_exposure_value(old_exposure);
		return;
	}

	camera_object->set_exposure_time_in_ms(new_exposure);
	current_exposure = new_exposure;
	is_lut_up_to_date = false;
	emit change_view_exposure_value(new_exposure);
}

void thermo_camera_model::gain_changed_by_user(int new_gain)
{
	int old_gain;
	camera_object->get_gain_percent(old_gain);

	if (new_gain == old_gain)
		return;

	int return_status = camera_object->set_gain_percent(new_gain);
	current_gain = new_gain;
	is_lut_up_to_date = false;
	emit change_view_gain_value(new_gain);
}

void thermo_camera_model::post_slot_connection_initialization()
{
	int received_camera_Gain;
	camera_object->get_gain_percent(received_camera_Gain);
	int received_camera_exposure;
	camera_object->get_exposure_time_in_ms(received_camera_exposure);
	cout << received_camera_exposure << endl;
	emit change_view_gain_value(received_camera_Gain);
	emit change_view_exposure_value(received_camera_exposure);
	current_gain = received_camera_Gain;
	current_exposure = received_camera_exposure;
}

void thermo_camera_model::emissivity_changed_by_user(double new_emissivity)
{
	emissivity = new_emissivity;
}

void thermo_camera_model::emit_measurement_picture()
{
	timestamp_t t0 = get_timestamp(); //

	//TODO needs to be changed later to support 2 color cameras the whole function (including emissivity support)
	unsigned char *data_pointer = get_data_pointer();

	float emissivity_snapshot;

	if (camera_object->is_monochrome()) {
		emissivity_snapshot = emissivity;
	} else {
		//in case camera is not monochrome we dont divide through emissivity since we use fraction
		emissivity_snapshot = 1;
	}

	int temperature;
//	if (!is_lut_up_to_date)
//	{
//		used_lut_table.clear();

//		for (int i = 0 ; i < 256 ; i++)
//		{
//			temperature = the_neural_network->get_temperature(current_gain, current_exposure, i);
//			used_lut_table.add_data(temperature, i);
//		}
//	}
	int j;
	float pixel_characteristic_value;

	float fraction;
	int value_to_lut;
	for (int i = 0 ; i < image_y ; ++i)
	{
		uchar *indexed_image_ptr = indexed_image_to_emit.scanLine(i);
		for ( j = 0 ; j < image_x ; ++j)
		{
			pixel_characteristic_value = (float) (*data_pointer);
			pixel_characteristic_value /= emissivity_snapshot;
			temperature = the_neural_network->get_temperature(current_gain, current_exposure, pixel_characteristic_value);
			current_temperature_2dvector[i][j] = temperature;


			fraction = temperature - minimum_temperature;
			fraction = fraction / (maximum_temperature - minimum_temperature);
			value_to_lut = (fraction*255) +0.5;
			if (value_to_lut > 255)
			{
				value_to_lut = 255;
			}
//			cout << " value_to_lut: " << value_to_lut << endl;
			*indexed_image_ptr = value_to_lut;
			indexed_image_ptr++;
			data_pointer = data_pointer +4;



		}
	}
timestamp_t t1 = get_timestamp();
	double secs = (t1 - t0) / 1000000.0L;
	if (for_cout_generation == 1)
	{
		cout << "t0: "<< t0<< endl;
		cout << "t1: "<< t1<< endl;
		cout << "secs: "<< secs<< endl<<endl;
		for_cout_generation =0;
	}
	for_cout_generation++;

	emit picture_changed(QPixmap::fromImage(indexed_image_to_emit));
}

void thermo_camera_model::qtimer_workaround()
{
	timer.start(10);
}

QImage thermo_camera_model::get_current_picture()
{
	uchar* uptr=(uchar*)get_data_pointer();
	QImage myImage(uptr,
				   camera_object->get_image_size_x(),
				   camera_object->get_image_size_y(),
				   QImage::Format_RGB32 );

	return myImage;
}

void thermo_camera_model::emit_calibration_picture()
{
	QImage myImage = get_current_picture();


	emit picture_changed(QPixmap::fromImage(myImage));
}

int thermo_camera_model::get_avarage_image_calibration_value(QImage image_to_calculate_value)
{
	int sum = 0;

	unsigned char *data_pointer;
	int image_heigh = image_to_calculate_value.height();
	int image_width = image_to_calculate_value.width();

	cout << "image_heigh" <<image_heigh << endl;
	cout << "image_width" << image_width << endl;
	for (int i = 0 ; i < image_heigh ; i ++)
	{
		data_pointer = image_to_calculate_value.scanLine(i);
		for (int j = 0 ; j < image_width ; j ++)
		{
			sum += (int) (*data_pointer);
			data_pointer += 4;
		}
	}
	int value_to_return = sum /(image_width*image_heigh);
	cout << "value_to_return: " << value_to_return << endl;
	return value_to_return;
}
