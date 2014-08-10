#include "inputs_file_generator.h"
#include "xml_handler.h"
#include <QTextStream>
#include "constants.h"

inputs_file_generator::inputs_file_generator()
{
}

void inputs_file_generator::fill_the_file(QFile &input_file)
{
	QTextStream fout(&input_file);
	int records_count = calibration_data_vector.size();
	fout << records_count << " 3 1" << endl;
	for (int i = 0 ; i < records_count ; i ++)
	{
		fout << (double)calibration_data_vector[i].gain /input_file_multiplier << " "
			 << (double)calibration_data_vector[i].exposure_time/input_file_multiplier << " "
			 << (double)calibration_data_vector[i].calibration_value/input_file_multiplier << endl;
		fout << (double)calibration_data_vector[i].temperature/input_file_multiplier << endl;
	}
}

void inputs_file_generator::generate_inputs_file(QString profile_name)
{
	calibration_data_vector = XML_handler().get_all_calibration_data_for_profile(profile_name);

	QFile input_file(profile_name.append(".data"));
	input_file.open(QIODevice::Truncate | QIODevice::ReadWrite | QIODevice::Text);

	fill_the_file(input_file);

	input_file.close();
}
