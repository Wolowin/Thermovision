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
#include <QRect>
#include "View/calibrationpicturedialog.h"
#include <vector>

class thermo_camera_model : public abstract_thermo_camera_model
{
public:
	thermo_camera_model();
	virtual ~thermo_camera_model();

	virtual unsigned char *get_data_pointer();

	virtual void create_new_profile(calibration_parameters the_parameters);
	virtual void run_measurement(neural_network_wrapper* tmp_neural_network);
	virtual void end_measurement();
	virtual void run_calibration(calibration_parameters the_calibration_parameters);
	virtual void start_calibration_video();
	virtual QImage get_current_picture();

public slots:
	virtual void exposure_changed_by_user(int new_exposure);
	virtual void gain_changed_by_user(int new_gain);
	virtual void post_slot_connection_initialization();
	virtual void emissivity_changed_by_user(double new_emissivity);

protected slots:
	virtual void emit_measurement_picture();

private:
	boost::scoped_ptr<abstract_camera_interface> camera_object;

	int get_avarage_image_calibration_value(QImage image_to_calculate_value);
	void do_calibration_step(int current_temp, std::map <int, int> &lut_temp_to_value_map);
	void qtimer_workaround();
	void emit_calibration_picture();

	QTimer timer;
	LUT_table used_lut_table;

	calibration_parameters the_calibration_parameters;
	CalibrationPictureDialog calibration_picture_dialog;
	std::vector< std::vector <int> > current_temperature_2dvector;
	float emissivity;
	neural_network_wrapper* the_neural_network;
	int current_gain;
	int current_exposure;
	QImage indexed_image_to_emit;
	int image_y;
	int image_x;

	const static int minimum_temperature = 0;
	const static int maximum_temperature = 1000;
	bool is_lut_up_to_date;
};
#endif // THERMO_CAMERA_MODEL_H
