#ifndef XML_HANDLER_H
#define XML_HANDLER_H

#include "abstract_xml_handler.h"
#include <QString>
#include <QFile>
#include <QDomDocument>



class XML_handler : public abstract_XML_handler
{
public:
	XML_handler();
	~XML_handler();
	virtual void add_profile(calibration_parameters the_parameters, std::map<int, int> lut_temp_to_value_map);
	virtual bool if_profile_already_exists(QString profile_name);
	virtual std::vector<calibration_parameters> getProfilesWithParameters();
	virtual LUT_table get_profile_LUT_table(QString profile);

protected:
	QString calibration_profiles_file_name;

private:
	QFile file;
	QDomDocument profiles_DOM_document;

	void rewrite_file();
};

#endif // XML_HANDLER_H
