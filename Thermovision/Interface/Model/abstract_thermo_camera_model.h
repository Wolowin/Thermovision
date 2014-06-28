#ifndef ABSTRACT_THERMO_CAMERA_MODEL_H
#define ABSTRACT_THERMO_CAMERA_MODEL_H

#include "QObject"
#include "Interface/calibration_parameters.h"
#include "Interface/lut_table.h"
#include <QPixmap>

class abstract_thermo_camera_model:public QObject
{
	Q_OBJECT

public:
	abstract_thermo_camera_model() {}
	virtual ~abstract_thermo_camera_model() {}

signals:
	void picture_changed(QPixmap new_image);
public slots:
	virtual void image_capture() = 0;
	virtual char* get_data_pointer() = 0;
	virtual void run_calibration(calibration_parameters the_parameters) = 0;
	virtual void run_measurement(LUT_table the_lut_table) = 0;
	virtual void end_measurement() = 0;

	virtual void react_to_changed_gain(int new_gain_percent) = 0;
	virtual void react_to_changed_exposure_time(int new_time_ms) = 0;

protected slots:
	virtual void emit_picture_changed() = 0;
};

#endif // ABSTRACT_THERMO_CAMERA_MODEL_H
