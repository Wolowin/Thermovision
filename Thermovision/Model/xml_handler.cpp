#include <iostream>


#include "xml_handler.h"
#include "log.h"

#include <QMessageBox>
#include <QTextStream>

//QString XML_handler::calibration_profiles_file_name = QString("profiles.xml");

XML_handler::XML_handler():
	calibration_profiles_file_name("profiles.xml"),
	file(calibration_profiles_file_name)
{
	if( !file.open( QIODevice::ReadWrite | QIODevice::Text ) )
	{
		log_debug( "Failed to open file for Read/Write." );
		return;
	}

	if( !profiles_DOM_document.setContent( &file ) )
	{
		log_debug("Failed to parse the file into a DOM tree.");
		file.close();
		return;
	}

}

XML_handler::~XML_handler()
{
	file.close();
}

void XML_handler::add_profile(calibration_parameters the_parameters)
{
	QDomElement root_element = profiles_DOM_document.documentElement();

	QDomElement new_profile = profiles_DOM_document.createElement(the_parameters.profile_name);
	QDomElement parameters = profiles_DOM_document.createElement( "Parameters" );

	parameters.setAttribute( "Camera_model", the_parameters.camera_model );
	parameters.setAttribute( "Filter_model", the_parameters.filter_model );
	parameters.setAttribute( "Distance_to_object", QString::number(the_parameters.distance_to_obj) );
	parameters.setAttribute( "Lens_focal", QString::number(the_parameters.lens_focal) );

	root_element.appendChild( new_profile );
	new_profile.appendChild(parameters);

	rewrite_file();
}

bool XML_handler::profile_exists(calibration_parameters parameters_to_check)
{
	QDomElement root_element = profiles_DOM_document.documentElement();

	if ( root_element.elementsByTagName( parameters_to_check.profile_name).size() != 0)
	{
		return true;
	}
	return false;
}

QList<QString> XML_handler::get_profiles_names()
{
	QDomElement first_child = profiles_DOM_document.firstChild().toElement();

	if (first_child == QDomElement())
	{
		log_debug("Document doesnt have first child! Incorrect file format.");
		return QList<QString>();
	}

	QList<QString> profile_names_vector;

	QDomNodeList profiles_list = first_child.childNodes();

	for (int i = 0; profiles_list.at(i) != QDomNode() ; i++)
	{
		profile_names_vector.push_back(profiles_list.at(i).toElement().tagName());
	}

	return profile_names_vector;
}

void XML_handler::iterate_the_profile(QDomNode profile,
									  std::vector<calibration_parameters> &parameters_vector)
{
	QDomElement profile_element;
	profile_element = profile.toElement();

	QDomElement parameters_element = profile_element.toElement().firstChildElement(QString("Parameters"));

	calibration_parameters the_parameters;
	the_parameters.profile_name = profile_element.tagName();

	the_parameters.camera_model = parameters_element.attribute(QString("Camera_model"));
	the_parameters.filter_model = parameters_element.attribute(QString("Filter_model"));
	the_parameters.distance_to_obj = parameters_element.attribute(QString("Distance_to_object")).toDouble();
	the_parameters.lens_focal = parameters_element.attribute(QString("Lens_focal")).toDouble();

	QDomNodeList gains_list = profile_element.childNodes();

	for (int i = 0; gains_list.at(i) != QDomNode() ; i++)
	{
		iterate_throught_gain(gains_list.at(i), parameters_vector, the_parameters);
	}
}

void XML_handler::iterate_throught_gain(QDomNode gain, std::vector<calibration_parameters> &parameters_vector,
										calibration_parameters the_parameters)
{
	QDomElement gain_element;
	gain_element = gain.toElement();

	QDomNodeList exposure_times_list = gain_element.childNodes();

	the_parameters.Gain = gain_element.tagName().remove("Gain_").toInt();

	for (int i = 0; exposure_times_list.at(i) != QDomNode() ; i++)
	{
		iterate_throught_exposure(exposure_times_list.at(i), parameters_vector, the_parameters);
	}

}

void XML_handler::iterate_throught_exposure(QDomNode exposure_time, std::vector<calibration_parameters> &parameters_vector,
											calibration_parameters the_parameters)
{
	QDomElement exposure_time_element = exposure_time.toElement();

	the_parameters.exposure_time = exposure_time_element.tagName().remove("Exposure_time_").toInt();

	parameters_vector.push_back(the_parameters);
}

std::vector<calibration_parameters> XML_handler::get_profiles_with_parameters()
{
	QDomElement first_child = profiles_DOM_document.firstChild().toElement();

	if (first_child == QDomElement())
	{
		log_debug("Document doesnt have first child! Incorrect file format.");
		return std::vector<calibration_parameters>();
	}

	std::vector<calibration_parameters> parameters_vector;

	QDomNodeList profiles_list = first_child.childNodes();

	for (int i = 0; profiles_list.at(i) != QDomNode() ; i++)
	{
		iterate_the_profile(profiles_list.at(i), parameters_vector);
	}

	return parameters_vector;
}

LUT_table XML_handler::get_profile_LUT_table(calibration_parameters profile_parameters)
{
	log_debug("Getting profile LUT table");
	LUT_table the_LUT_table;
	if (profiles_DOM_document.firstChildElement("Profiles").firstChildElement(profile_parameters.profile_name) == QDomElement())
	{
		log_debug("Didnt find element for some reason");
		return LUT_table();
	}

	QDomElement profile_element =
			profiles_DOM_document.firstChildElement("Profiles").firstChildElement(profile_parameters.profile_name);

	QString gain_string = QString("Gain_").append(QString::number(profile_parameters.Gain));
	QDomElement gain_element = profile_element.firstChildElement(gain_string);

	QString exposure_string = QString("Exposure_time_").append(QString::number(profile_parameters.exposure_time));
	QDomElement exposure_element = gain_element.firstChildElement(exposure_string);

	QDomNamedNodeMap lut_attributes =
			exposure_element.firstChildElement("LUT").attributes();

	int temperatures_count = lut_attributes.count();
	for (int i = 0 ; i < temperatures_count ; i ++)
	{
		QDomAttr current_attribute = lut_attributes.item(i).toAttr();
		QString attribute_name = current_attribute.name().remove(QChar('t'));
		QString attribute_value = current_attribute.value();

		the_LUT_table.add_data_from_profile(attribute_name.toInt(), attribute_value.toDouble());
	}

	return the_LUT_table;
}

calibration_parameters XML_handler::get_profile_details(QString profile_name)
{
	QDomElement root_element = profiles_DOM_document.documentElement();
	QDomElement profile_element = root_element.firstChildElement(profile_name);
	QDomElement parameters_element = profile_element.firstChildElement(QString("Parameters"));

	calibration_parameters the_parameters;
	the_parameters.profile_name = profile_name;
	the_parameters.camera_model = parameters_element.attribute(QString("Camera_model"));
	the_parameters.filter_model = parameters_element.attribute(QString("Filter_model"));
	the_parameters.distance_to_obj = parameters_element.attribute(QString("Distance_to_object")).toDouble();
	the_parameters.lens_focal = parameters_element.attribute(QString("Lens_focal")).toDouble();
	return the_parameters;
}

void XML_handler::add_lut_attribute(int temperature, int value, QDomElement &lut)
{
	QString string_temp = QString("t").append(QString::number(temperature));
	QString string_value = QString::number(value);
	lut.setAttribute(string_temp, string_value);
}

void XML_handler::add_calibration_outcome(calibration_parameters parameters, int temperature, int value)
{
	log_debug("Siemka");
	QDomElement root_element = profiles_DOM_document.documentElement();
	QDomElement profile_element = root_element.firstChildElement(parameters.profile_name);
	QString gain_string = QString("Gain_").append(QString::number(parameters.Gain));
	QString exposure_string = QString("Exposure_time_").append(QString::number(parameters.exposure_time));

	QDomElement gain_element = profile_element.firstChildElement(gain_string);
	if ( gain_element == QDomElement() )
	{
		log_debug("Dodaje nowy element gain");
		//add new gain and exposure element;
		QDomElement gain = profiles_DOM_document.createElement(gain_string);
		QDomElement exposure = profiles_DOM_document.createElement(exposure_string);
		QDomElement lut = profiles_DOM_document.createElement( "LUT" );

		profile_element.appendChild(gain);
		gain.appendChild(exposure);
		exposure.appendChild(lut);

		add_lut_attribute(temperature, value, lut);
	}
	else
	{
		QDomElement exposure_element = gain_element.firstChildElement(exposure_string);
		if ( exposure_element == QDomElement() )
		{
			log_debug("Dodaje nowy element exposure");
			QDomElement exposure = profiles_DOM_document.createElement(exposure_string);
			QDomElement lut = profiles_DOM_document.createElement( "LUT" );

			gain_element.appendChild(exposure);
			exposure.appendChild(lut);

			add_lut_attribute(temperature, value, lut);
		}
		else
		{
			log_debug("Dodaje nowy wpis lut");
			QDomElement lut = exposure_element.firstChildElement( "LUT" );
			add_lut_attribute(temperature, value, lut);
		}
	}
		rewrite_file();
}

void XML_handler::rewrite_file()
{
	file.resize(0);
	QTextStream stream( &file );
	stream << profiles_DOM_document.toString();
}
