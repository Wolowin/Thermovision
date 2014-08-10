#ifndef ABSTRACT_THERMO_CAMERA_MODEL_H
#define ABSTRACT_THERMO_CAMERA_MODEL_H

#include "QObject"
#include "Interface/calibration_parameters.h"
#include "Interface/lut_table.h"
#include <QPixmap>


class neural_network_wrapper;

class abstract_thermo_camera_model:public QObject
{
	Q_OBJECT

public:
	abstract_thermo_camera_model() {}
	virtual ~abstract_thermo_camera_model() {}

signals:
	void picture_changed(QPixmap new_image);
	void change_view_gain_value(int new_gain);
	void change_view_exposure_value(int new_exposure);

public slots:
	virtual unsigned char *get_data_pointer() = 0;
	virtual void create_new_profile(calibration_parameters the_parameters) = 0;
	virtual void run_measurement(neural_network_wrapper* the_neural_network) = 0;
	virtual void end_measurement() = 0;
	virtual void run_calibration(calibration_parameters the_calibration_parameters) = 0;
	virtual void start_calibration_video() = 0;
	virtual void exposure_changed_by_user(int new_exposure) = 0;
	virtual void gain_changed_by_user(int new_gain) = 0;
	virtual void post_slot_connection_initialization() = 0;
	virtual QImage get_current_picture() = 0;
	virtual void emissivity_changed_by_user(double new_emissivity) = 0;

protected slots:
	virtual void emit_measurement_picture() = 0;
	virtual void emit_calibration_picture() = 0;
};

#endif // ABSTRACT_THERMO_CAMERA_MODEL_H
