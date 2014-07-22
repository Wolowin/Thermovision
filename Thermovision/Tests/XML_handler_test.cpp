#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <Qstring>
#include "Model/xml_handler.h"
#include <QList>

TEST(XML_handler_test, get_profiles_names)
{
	XML_handler the_handler;
	QList<QString> received_profile_names_vector;
	QList<QString> expected_profiles_names_vector;

	expected_profiles_names_vector.push_back(QString("Lutowanie"));
	expected_profiles_names_vector.push_back(QString("Hartowanie"));
	received_profile_names_vector = the_handler.get_profiles_names();

	EXPECT_EQ(2, received_profile_names_vector.size());
	EXPECT_EQ(expected_profiles_names_vector, received_profile_names_vector);

}

TEST(XML_handler_test, profile_exists)
{
	XML_handler the_handler;
	calibration_parameters existing_parameters;
	existing_parameters.profile_name = QString("Lutowanie");
	existing_parameters.Gain = 30;
	existing_parameters.exposure_time = 50;
	existing_parameters.camera_model = QString("b632");
	existing_parameters.filter_model = QString("6n423");
	existing_parameters.distance_to_obj = 64;
	existing_parameters.lens_focal = 2;

	EXPECT_EQ(true, the_handler.profile_exists(existing_parameters));

	calibration_parameters not_existing_parameters_profile;
	not_existing_parameters_profile.profile_name = QString("NieLutowanie");
	not_existing_parameters_profile.Gain = 15;
	not_existing_parameters_profile.exposure_time = 52;
	not_existing_parameters_profile.camera_model = QString("b632");
	not_existing_parameters_profile.filter_model = QString("6n423");
	not_existing_parameters_profile.distance_to_obj = 64;
	not_existing_parameters_profile.lens_focal = 2;

	EXPECT_EQ(false, the_handler.profile_exists(not_existing_parameters_profile));
}

TEST(XML_handler_test, get_profile_details)
{
	XML_handler the_handler;

	calibration_parameters expected_parameters;
	expected_parameters.profile_name = QString("Lutowanie");
	expected_parameters.camera_model = QString("b632");
	expected_parameters.filter_model = QString("6n423");
	expected_parameters.distance_to_obj = 64;
	expected_parameters.lens_focal = 2;


	calibration_parameters received_parameters = the_handler.get_profile_details(QString("Lutowanie"));
//	EXPECT_EQ(expected_parameters, received_parameters);

	EXPECT_EQ(expected_parameters.profile_name.toStdString(), received_parameters.profile_name.toStdString());
	EXPECT_EQ(expected_parameters.camera_model.toStdString(), received_parameters.camera_model.toStdString());
	EXPECT_EQ(expected_parameters.filter_model.toStdString(), received_parameters.filter_model.toStdString());
	EXPECT_EQ(expected_parameters.distance_to_obj, received_parameters.distance_to_obj);
	EXPECT_EQ(expected_parameters.lens_focal, received_parameters.lens_focal);
}

TEST(DISABLED_XML_handler_test, get_profiles_with_parameters)
{
//	QFile::copy(QString("profiles.xml"), QString("copied.xml"));

	{
		XML_handler the_handler;

		std::vector<calibration_parameters> received_parameters =
				the_handler.get_profiles_with_parameters();

		EXPECT_EQ(5,received_parameters.size());

		std::vector<calibration_parameters> expected_parameters_list;
		calibration_parameters expected_parameters;

		expected_parameters.profile_name = QString("Lutowanie");
		expected_parameters.Gain = 15;
		expected_parameters.exposure_time = 52;
		expected_parameters.camera_model = QString("b632");
		expected_parameters.filter_model = QString("6n423");
		expected_parameters.distance_to_obj = 64;
		expected_parameters.lens_focal = 2;
		expected_parameters_list.push_back(expected_parameters);

		EXPECT_EQ(expected_parameters_list[0].profile_name, received_parameters[0].profile_name);
		EXPECT_EQ(expected_parameters_list[0].Gain, received_parameters[0].Gain);
		EXPECT_EQ(expected_parameters_list[0].exposure_time, received_parameters[0].exposure_time);
		EXPECT_EQ(expected_parameters_list[0].camera_model, received_parameters[0].camera_model);
		EXPECT_EQ(expected_parameters_list[0].filter_model, received_parameters[0].filter_model);
		EXPECT_EQ(expected_parameters_list[0].distance_to_obj, received_parameters[0].distance_to_obj);
		EXPECT_EQ(expected_parameters_list[0].lens_focal, received_parameters[0].lens_focal);

		EXPECT_EQ(expected_parameters_list[0], received_parameters[0]);

		expected_parameters.profile_name = QString("Lutowanie");
		expected_parameters.Gain = 15;
		expected_parameters.exposure_time = 64;
		expected_parameters.camera_model = QString("rty");
		expected_parameters.filter_model = QString("qwe");
		expected_parameters.distance_to_obj = 64;
		expected_parameters.lens_focal = 2;
		expected_parameters_list.push_back(expected_parameters);
		EXPECT_EQ(expected_parameters_list[1], received_parameters[1]);


		expected_parameters.profile_name = QString("Hartowanie");
		expected_parameters.Gain = 30;
		expected_parameters.exposure_time = 20;
		expected_parameters.camera_model = QString("SD-161-J");
		expected_parameters.filter_model = QString("LT-AS-5");
		expected_parameters.distance_to_obj = 10;
		expected_parameters.lens_focal = 0.1;

		EXPECT_EQ(expected_parameters, received_parameters[4]);

		//	expected_parameters.profile_name = ;
		//	expected_parameters.Gain = ;
		//	expected_parameters.exposure_time = ;
		//	expected_parameters.camera_model = ;
		//	expected_parameters.filter_model = ;
		//	expected_parameters.distance_to_obj =;
		//	expected_parameters.lens_focal = ;
		//	expected_parameters_list.push_back(expected_parameters);

		//	expected_parameters.profile_name = ;
		//	expected_parameters.Gain = ;
		//	expected_parameters.exposure_time = ;
		//	expected_parameters.camera_model = ;
		//	expected_parameters.filter_model = ;
		//	expected_parameters.distance_to_obj =;
		//	expected_parameters.lens_focal = ;
		//	expected_parameters_list.push_back(expected_parameters);
	}
//QFile::remove(QString("profiles.xml"));
//QFile::copy(QString("copied.xml"), QString("profiles.xml"));
//QFile::remove(QString("copied.xml"));


}
