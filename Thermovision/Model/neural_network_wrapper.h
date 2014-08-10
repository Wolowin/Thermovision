#ifndef NEURAL_NETWORK_WRAPPER_H
#define NEURAL_NETWORK_WRAPPER_H
#include <QString>
#include "fann.h"

class neural_network_wrapper
{
public:
	neural_network_wrapper();
	~neural_network_wrapper();
	void reteach_the_network(QString profile_name);
	int get_temperature(int gain, int exposure, float value);

private:
	void check_if_destroying_network_needed();
	struct fann *ann;
	bool is_network_initialized;
	fann_type *calculated_output;
	fann_type network_input_array[3];
	int temperature_to_return;

	int for_cout_generation;


};

#endif // NEURAL_NETWORK_WRAPPER_H
