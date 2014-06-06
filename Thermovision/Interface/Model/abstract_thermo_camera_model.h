#ifndef ABSTRACT_THERMO_CAMERA_MODEL_H
#define ABSTRACT_THERMO_CAMERA_MODEL_H

#include "QObject"
#include "Interface/calibration_parameters.h"

class abstract_thermo_camera_model:public QObject
{
	Q_OBJECT

public:
	abstract_thermo_camera_model() {}
	virtual ~abstract_thermo_camera_model() {}

public slots:
	virtual void image_capture() = 0;
	virtual char* get_data_pointer() = 0;
	virtual void run_calibration(calibration_parameters the_parameters) = 0;
};

#endif // ABSTRACT_THERMO_CAMERA_MODEL_H
