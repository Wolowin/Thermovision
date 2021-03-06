#ifndef CAMERA_FACTORY_H
#define CAMERA_FACTORY_H

#include "abstract_camera_interface.h"

class camera_factory
{
public:
	camera_factory();
	virtual ~camera_factory();

	static abstract_camera_interface* get_camera_object();

};

#endif // CAMERA_FACTORY_H
