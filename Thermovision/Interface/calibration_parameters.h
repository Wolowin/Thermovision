#ifndef CALIBRATION_PARAMETERS_H
#define CALIBRATION_PARAMETERS_H

#include <QString>

struct calibration_parameters
{
	QString profile_name;
	QString camera_model;
	QString filter_model;
	double distance_to_obj;
	double lens_focal;
	int Gain;
	int exposure_time;
};

#endif // CALIBRATION_PARAMETERS_H
