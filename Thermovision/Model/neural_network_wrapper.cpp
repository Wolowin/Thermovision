#include "neural_network_wrapper.h"
#include "fann.h"
#include <QString>
#include "inputs_file_generator.h"
#include <iostream>
#include "log.h"
#include "constants.h"

using namespace std;



neural_network_wrapper::neural_network_wrapper():
	is_network_initialized(false)
{
	for_cout_generation = 0;
}

neural_network_wrapper::~neural_network_wrapper()
{
	check_if_destroying_network_needed();
}

void neural_network_wrapper::check_if_destroying_network_needed()
{
	if (is_network_initialized)
	{
		log_debug("Destroying old neural network");
		fann_destroy(ann);
	}
}

void neural_network_wrapper::reteach_the_network(QString profile_name)
{
	check_if_destroying_network_needed();

	log_debug("Reteaching the network");
	inputs_file_generator().generate_inputs_file(profile_name);

	const unsigned int num_input = 3;
	const unsigned int num_output = 1;
	const unsigned int num_layers = 3;
	const unsigned int num_neurons_hidden = 4;
	const float desired_error = (const float) 0.001;
	const unsigned int max_epochs = 50000;
	const unsigned int epochs_between_reports = 1000;

	ann = fann_create_standard(num_layers, num_input, num_neurons_hidden, num_output);
	is_network_initialized = true;

	fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
	fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

	log_debug("training ann");
	fann_train_on_file(ann,
					   profile_name.append(".data").toStdString().c_str(),
					   max_epochs,
					   epochs_between_reports,
					   desired_error);

}

int neural_network_wrapper::get_temperature(int gain, int exposure, float value)
{
	if (is_network_initialized)
	{
//		log_debug("Getting temperature");
		network_input_array[0] = (float)gain / input_file_multiplier;
		network_input_array[1] = (float)exposure/ input_file_multiplier;
		network_input_array[2] = value / input_file_multiplier;
		calculated_output = fann_run(ann, network_input_array);
		temperature_to_return = *calculated_output * input_file_multiplier;
//		cout << "Wynik: " << *calc_out << " input0: " << input[0] << " input1: " << input[1] << " input2: "<< input[2] << endl;

//		if (for_cout_generation == 500000)
//		{
//			cout << "gain: " << gain << endl;
//			cout << "exposure: " << exposure << endl;
//			cout << "value: " << value << endl;
//			cout << "temperature: " << temperature << endl;
//			for_cout_generation = 0;
//		}
//		for_cout_generation ++;
		//	fann_destroy(ann);

		return temperature_to_return;
	}
	else
		return 0;
}
