#ifndef ABSTRACT_CAMERA_INTERFACE_H
#define ABSTRACT_CAMERA_INTERFACE_H

class abstract_camera_interface
{
public:
	abstract_camera_interface();
	virtual ~abstract_camera_interface();

	virtual void initialize_camera() = 0;
	virtual char* get_data_pointer() = 0;
	virtual void capture_picture() = 0;
	virtual void start_live_video() = 0;

};

#endif // ABSTRACT_CAMERA_INTERFACE_H
