#ifndef XML_HANDLER_H
#define XML_HANDLER_H

#include "abstract_xml_handler.h"
#include <QString>
#include <QFile>
#include <QDomDocument>
#include "Interface/calibration_parameters.h"


class XML_handler : public abstract_XML_handler
{
public:
	XML_handler();
	~XML_handler();
	virtual void add_profile(calibration_parameters the_parameters);
	virtual bool profile_exists(calibration_parameters parameters_to_check);
	virtual QList<QString> get_profiles_names();
	virtual std::vector<calibration_parameters> get_profiles_with_parameters();
	virtual LUT_table get_profile_LUT_table(calibration_parameters profile_parameters);
	virtual calibration_parameters get_profile_details(QString profile_name);
	virtual void add_calibration_outcome(
			calibration_parameters parameters, int temperature, int value);

protected:
	QString calibration_profiles_file_name;

private:
	QFile file;
	QDomDocument profiles_DOM_document;

	void add_lut_attribute(int temperature, int value, QDomElement &lut);
	void rewrite_file();
	void iterate_the_profile(QDomNode profile, std::vector<calibration_parameters> &parameters_vector);
	void iterate_throught_gain(QDomNode gain,
							   std::vector<calibration_parameters> &parameters_vector, calibration_parameters the_parameters);
	void iterate_throught_exposure(QDomNode exposure_time, std::vector<calibration_parameters> &parameters_vector,
								   calibration_parameters the_parameters);
};

#endif // XML_HANDLER_H
