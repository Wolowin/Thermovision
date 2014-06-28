#ifndef ABSTRACT_XML_HANDLER_H
#define ABSTRACT_XML_HANDLER_H

#include "Interface/calibration_parameters.h"
#include <map>
#include <vector>
#include "Interface/lut_table.h"

class abstract_XML_handler
{
public:
	abstract_XML_handler() {}
	virtual ~abstract_XML_handler() {}
	virtual void add_profile(calibration_parameters the_parameters, std::map<int, int> lut_temp_to_value_map) = 0;
	virtual bool if_profile_already_exists(QString profile_name) = 0;
	virtual std::vector<calibration_parameters> getProfilesWithParameters() = 0;
	virtual LUT_table get_profile_LUT_table(QString profile) = 0;
};

#endif // ABSTRACT_XML_HANDLER_H
