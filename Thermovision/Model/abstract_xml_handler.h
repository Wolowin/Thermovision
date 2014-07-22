#ifndef ABSTRACT_XML_HANDLER_H
#define ABSTRACT_XML_HANDLER_H

#include "Interface/calibration_parameters.h"
#include <map>
#include <vector>
#include "Interface/lut_table.h"
#include <QList>

class abstract_XML_handler
{
public:
	abstract_XML_handler() {}
	virtual ~abstract_XML_handler() {}
	virtual void add_profile(calibration_parameters the_parameters) = 0;
	virtual bool profile_exists(calibration_parameters parameters_to_check) = 0;
	virtual std::vector<calibration_parameters> get_profiles_with_parameters() = 0;
	virtual LUT_table get_profile_LUT_table(calibration_parameters profile_parameters) = 0;
	virtual QList<QString> get_profiles_names() = 0;
	virtual calibration_parameters get_profile_details(QString profile_name) = 0;
	virtual void add_calibration_outcome(
			calibration_parameters parameters, int temperature, int value) = 0;
};

#endif // ABSTRACT_XML_HANDLER_H
