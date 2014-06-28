#ifndef THERMO_CAMERA_MODEL_H
#define THERMO_CAMERA_MODEL_H

#include "Interface/calibration_parameters.h"
#include "../Interface/Model/abstract_thermo_camera_model.h"
#include "Interface/Camera/abstract_camera_interface.h"
#include <QDomDocument>
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <QImage>
#include <QTimer>

class thermo_camera_model : public abstract_thermo_camera_model
{
public:
	thermo_camera_model();
	virtual ~thermo_camera_model();

	virtual char* get_data_pointer();
	virtual void image_capture();

	virtual void run_calibration(calibration_parameters the_parameters);
	virtual void run_measurement(LUT_table the_lut_table);
	virtual void end_measurement();

public slots:
	virtual void react_to_changed_gain(int new_gain_percent);
	virtual void react_to_changed_exposure_time(int new_time_ms);

protected slots:
	virtual void emit_picture_changed();

private:
	boost::scoped_ptr<abstract_camera_interface> camera_object;

	static const int calibration_start_temp = 200;
	static const int calibration_end_temp = 1600;
	static const int calibration_temp_increment = 50;

//	QImage qimage_from_32grayscale()
	int value_from_calibration();
	void do_calibration_step(int current_temp, std::map <int, int> &lut_temp_to_value_map);
	void qtimer_workaround();
	void emit_calibration_picture();

	QTimer timer;
	LUT_table used_lut_table;

	static int number_of_picture;
	calibration_parameters the_calibration_parameters;
};
#endif // THERMO_CAMERA_MODEL_H
