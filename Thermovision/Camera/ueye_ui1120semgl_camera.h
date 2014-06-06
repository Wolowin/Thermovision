#ifndef UEYE_UI1120SEMGL_CAMERA_H
#define UEYE_UI1120SEMGL_CAMERA_H

class uEYE_UI1120SEMGL_camera : public abstract_camera_interface
{
public:
	uEYE_UI1120SEMGL_camera();

	virtual void initialize_camera();
	virtual char* get_data_pointer();
	virtual void capture_picture();
	virtual void start_live_video();
};

#endif // UEYE_UI1120SEMGL_CAMERA_H
