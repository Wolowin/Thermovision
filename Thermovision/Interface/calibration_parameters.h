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

	bool operator==(const calibration_parameters other_parameters) const
	{
		if (profile_name != other_parameters.profile_name)
			return false;
		if (camera_model != other_parameters.camera_model)
			return false;
		if (filter_model != other_parameters.filter_model)
			return false;
		if (distance_to_obj != other_parameters.distance_to_obj)
			return false;
		if (lens_focal != other_parameters.lens_focal)
			return false;
		if (Gain != other_parameters.Gain)
			return false;
		if (exposure_time != other_parameters.exposure_time)
			return false;
		return true;
	}
};

#endif // CALIBRATION_PARAMETERS_H
