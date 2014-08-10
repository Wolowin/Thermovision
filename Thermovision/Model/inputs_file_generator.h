#ifndef INPUTS_FILE_GENERATOR_H
#define INPUTS_FILE_GENERATOR_H

#include <QString>
#include <vector>
#include "Interface/calibrations_data.h"
#include "QFile"

class inputs_file_generator
{
public:
	inputs_file_generator();
	void generate_inputs_file(QString profile_name);

private:
	std::vector<calibration_data> calibration_data_vector;

	void fill_the_file(QFile &input_file);
};

#endif // INPUTS_FILE_GENERATOR_H
