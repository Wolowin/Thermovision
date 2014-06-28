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

int thermo_camera_model::number_of_picture = 1;

thermo_camera_model::thermo_camera_model():
	camera_object(camera_factory::get_camera_object())
{
	camera_object->initialize_camera();
	connect(&timer, SIGNAL(timeout()), this, SLOT(emit_picture_changed()));
}

thermo_camera_model::~thermo_camera_model()
{
	camera_object->deinitialize_camera();
}

char *thermo_camera_model::get_data_pointer()
{
	return camera_object->get_data_pointer();
}


void thermo_camera_model::image_capture()
{
	camera_object->start_live_video();

}

void thermo_camera_model::run_calibration(calibration_parameters the_parameters)
{
	the_calibration_parameters = the_parameters;
	boost::scoped_ptr<abstract_XML_handler> the_xml_handler(new XML_handler);

	if (the_xml_handler->if_profile_already_exists(the_calibration_parameters.profile_name))
	{
		return;
	}

	std::map <int, int> lut_temp_to_value_map;

	for (int current_temp = calibration_start_temp;
		current_temp <= calibration_end_temp;
		current_temp += calibration_temp_increment)
	{
		do_calibration_step(current_temp, lut_temp_to_value_map);
	}

	the_xml_handler->add_profile(the_calibration_parameters, lut_temp_to_value_map);
}

void thermo_camera_model::run_measurement(LUT_table the_lut_table)
{
	camera_object->start_live_video();
	used_lut_table = the_lut_table;
	qtimer_workaround();
}

void thermo_camera_model::end_measurement()
{
	camera_object->stop_live_video();
	timer.stop();
}

void thermo_camera_model::react_to_changed_gain(int new_gain_percent)
{
//	if (new_gain_percent < 0 ||)
//	the_calibration_parameters.
}

void thermo_camera_model::react_to_changed_exposure_time(int new_time_ms)
{

}

void thermo_camera_model::emit_picture_changed()
{
	//TODO needs to be changed later to support 2 color cameras
	char *data_pointer = get_data_pointer();

	int minimum_temperature = 200;
	int maximum_temperature = 1000;


	int image_heigh = camera_object->get_image_size_y();
	int image_width = camera_object->get_image_size_x();

	QImage indexed_image (image_width, image_heigh, QImage::Format_Indexed8);

	for (int i = 0 ; i < image_heigh ; i ++)
	{
		uchar *indexed_image_ptr = indexed_image.scanLine(i);
		for (int j = 0 ; j < image_width ; j ++)
		{
			double pixel_characteristic_value = (double) ((uchar)*data_pointer);
			int temperature = used_lut_table.get_temp_from_value(pixel_characteristic_value);
//			cout << "Dla wartosci pixel_characteristic_value: " << pixel_characteristic_value << " wartosc temperatury wynosi: " << temperature << endl;
			double fraction = temperature - minimum_temperature;
			fraction = fraction / (maximum_temperature - minimum_temperature);
//			cout << " fraction: " << fraction << endl;
			int value_to_lut = (fraction*255) +0.5;
//			cout << " value_to_lut: " << value_to_lut << endl;
			*indexed_image_ptr = value_to_lut;
			indexed_image_ptr++;
			data_pointer = data_pointer +4;
//			Sleep(500);
		}
	}

	indexed_image.setColorTable(LUT_table::false_color_qcolortable);

	emit picture_changed(QPixmap::fromImage(indexed_image));
}

void thermo_camera_model::do_calibration_step(int current_temp, std::map <int, int> &lut_temp_to_value_map)
{
	std::stringstream ss;
	ss << "Set the Black Body temperature to: " << current_temp << endl << endl << "When the temperature is set, please click OK";
	QMessageBox::warning(0, "ACTION REQUIRED!!!", ss.str().c_str());

	camera_object->capture_picture();

	emit_calibration_picture();

	lut_temp_to_value_map.insert(std::make_pair(current_temp, value_from_calibration()));
}

void thermo_camera_model::qtimer_workaround()
{
	timer.start(10);
}

void thermo_camera_model::emit_calibration_picture()
{
	uchar* uptr=(uchar*)get_data_pointer();
	QImage myImage(uptr,
				   camera_object->get_image_size_x(),
				   camera_object->get_image_size_y(),
				   QImage::Format_RGB32 );

	//saving the picture
	QString name = QString::number(number_of_picture++);
	name.append(".jpg");
	myImage.save(name);
	emit picture_changed(QPixmap::fromImage(myImage));
}

int thermo_camera_model::value_from_calibration()
{
	int sum = 0;
	char *data_pointer = get_data_pointer();
	int image_heigh = camera_object->get_image_size_y();
	int image_width = camera_object->get_image_size_x();
	for (int i = 0 ; i < image_heigh ; i ++)
	{
		for (int j = 0 ; j < image_width ; j ++)
		{
			sum += (int) ((uchar)*data_pointer);
			data_pointer += 4;
		}
	}
	return sum /(image_width*image_heigh);
}
