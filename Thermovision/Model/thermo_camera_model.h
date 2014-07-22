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

class thermo_camera_model : public abstract_thermo_camera_model
{
public:
	thermo_camera_model();
	virtual ~thermo_camera_model();

	virtual char* get_data_pointer();

	virtual void create_new_profile(calibration_parameters the_parameters);
	virtual void run_measurement(LUT_table the_lut_table);
	virtual void end_measurement();
	virtual void run_calibration(calibration_parameters the_calibration_parameters);
	virtual void start_calibration_video();
	virtual QImage get_current_picture();

public slots:
	virtual void exposure_changed_by_user(int new_exposure);
	virtual void gain_changed_by_user(int new_gain);
	virtual void post_slot_connection_initialization();

protected slots:
	virtual void emit_measurement_picture();

private:
	boost::scoped_ptr<abstract_camera_interface> camera_object;

//	QImage qimage_from_32grayscale()
	int get_avarage_image_calibration_value(QImage image_to_calculate_value);
	void do_calibration_step(int current_temp, std::map <int, int> &lut_temp_to_value_map);
	void qtimer_workaround();
	void emit_calibration_picture();

	QTimer timer;
	LUT_table used_lut_table;

	static int number_of_picture;
	calibration_parameters the_calibration_parameters;
	CalibrationPictureDialog calibration_picture_dialog;

};
#endif // THERMO_CAMERA_MODEL_H
