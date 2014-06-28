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

void XML_handler::add_profile(calibration_parameters the_parameters, map<int, int> lut_temp_to_value_map)
{
	if ( if_profile_already_exists(the_parameters.profile_name))
	{
		return;
	}
	QDomElement root_element = profiles_DOM_document.documentElement();

	QDomElement new_profile = profiles_DOM_document.createElement(the_parameters.profile_name);
	QDomElement parameters = profiles_DOM_document.createElement( "Parameters" );
	QDomElement lut = profiles_DOM_document.createElement( "LUT" );
	parameters.setAttribute( "Camera_model", the_parameters.camera_model );
	parameters.setAttribute( "Filter_model", the_parameters.filter_model );
	parameters.setAttribute( "Distance_to_object", QString::number(the_parameters.distance_to_obj) );
	parameters.setAttribute( "Lens_focal", QString::number(the_parameters.lens_focal) );
	parameters.setAttribute( "Gain", QString::number(the_parameters.Gain) );
	parameters.setAttribute( "Exposure_time", QString::number(the_parameters.exposure_time));

	map<int, int>::iterator it;
	for (it = lut_temp_to_value_map.begin();
		 it != lut_temp_to_value_map.end();
		 it++)
	{
		std::string current_temp_str = "t" + std::to_string(it->first);
		std::string pixel_value_str = std::to_string(it->second);

		lut.setAttribute(current_temp_str.c_str(), pixel_value_str.c_str() );
	}

	root_element.appendChild( new_profile );
	new_profile.appendChild(parameters);
	new_profile.appendChild(lut);

	rewrite_file();
}

bool XML_handler::if_profile_already_exists(QString profile_name)
{
	QDomElement root_element = profiles_DOM_document.documentElement();

	if ( root_element.elementsByTagName( profile_name).size() != 0)
	{
		QMessageBox::warning(0, "Communique", "Profile name already exists in the database!");
		log_debug("Profile name already exists in the database");
		return true;
	}
	return false;
}

std::vector<calibration_parameters> XML_handler::getProfilesWithParameters()
{
	QDomElement first_child = profiles_DOM_document.firstChild().toElement();

	if (first_child == QDomElement())
	{
		//TODO add some log
		cout << "XXX" << endl;
		return std::vector<calibration_parameters>();
	}

	QDomNodeList node_list = first_child.childNodes();
	QDomElement tmp;
	std::vector<calibration_parameters> parameters_vector;
	for (int i = 0; node_list.at(i) != QDomNode() ; i++)
	{
		calibration_parameters the_parameters;

		tmp = node_list.at(i).toElement();
		the_parameters.profile_name = tmp.tagName();

		tmp = tmp.firstChildElement(QString("Parameters"));

		the_parameters.camera_model = tmp.attribute(QString("Camera_model"));
		the_parameters.filter_model = tmp.attribute(QString("Filter_model"));
		the_parameters.distance_to_obj = tmp.attribute(QString("Distance_to_object")).toDouble();
		the_parameters.lens_focal = tmp.attribute(QString("Lens_focal")).toDouble();
		the_parameters.Gain = tmp.attribute(QString("Gain")).toInt();
		the_parameters.exposure_time = tmp.attribute(QString("Exposure_time")).toInt();
		parameters_vector.push_back(the_parameters);
	}

	QString tmp2 = first_child.tagName();
	cout << tmp2.toStdString() << endl;
	cout << parameters_vector.size() << endl;
	return parameters_vector;
}

LUT_table XML_handler::get_profile_LUT_table(QString profile)
{
	log_debug("Getting profile LUT table");
	LUT_table the_LUT_table;
	if (profiles_DOM_document.firstChildElement("Profiles").firstChildElement(profile) == QDomElement())
	{
		log_debug("Didnt find element for some reason");
		return LUT_table();
	}

	QDomNamedNodeMap lut_attributes =
			profiles_DOM_document.firstChildElement("Profiles").firstChildElement(profile)
			.firstChildElement("LUT").attributes();

	int temperatures_count = lut_attributes.count();
	for (int i = 0 ; i < temperatures_count ; i ++)
	{
		QDomAttr current_attribute = lut_attributes.item(i).toAttr();
		QString attribute_name = current_attribute.name().remove(QChar('t'));
		QString attribute_value = current_attribute.value();

		cout << attribute_name.toStdString() << endl;
		cout << attribute_value.toStdString() << endl << endl;
		the_LUT_table.add_data_from_profile(attribute_name.toInt(), attribute_value.toDouble());
	}

	return the_LUT_table;
}

void XML_handler::rewrite_file()
{
	file.resize(0);
	QTextStream stream( &file );
	stream << profiles_DOM_document.toString();
}
