#ifndef UEYE_UI1490SEMGL_CAMERA_H
#define UEYE_UI1490SEMGL_CAMERA_H

#include "Interface/Camera/abstract_camera_interface.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include <uEye.h>
class UEYE_UI1490SEMGL_camera : public abstract_camera_interface
{
public:
	UEYE_UI1490SEMGL_camera();

	virtual void initialize_camera();
	virtual void deinitialize_camera();
	virtual char* get_data_pointer();
	virtual void capture_picture();
	virtual void start_live_video();
	virtual int get_image_size_x();
	virtual int get_image_size_y();
	virtual void stop_live_video();
	virtual int set_gain_percent(int gain_percent);
	virtual int set_exposure_time_in_ms(int exposure_time_ms);
	virtual int get_gain_percent(int &gain_percent);
	virtual int get_exposure_time_in_ms(int &exposure_time_ms);
	virtual bool is_monochrome();
private:

	void get_connected_cameras_infos();
	void alloc_mem_for_camera_list();
	void get_camera_list();
	void get_sensor_infos();
	void init_camera();
	void check_if_firmware_update_needed(INT nRet);
	void allocate_memory_for_image();
	void GetMaxImageSize();


	static const HIDS starting_camera_handle = 0;
	HIDS operating_camera_handle;
	INT number_of_cameras;
	boost::shared_ptr<UEYE_CAMERA_LIST> camera_list_ptr;
	boost::scoped_ptr<SENSORINFO> sensor_info_ptr;

	int image_memory_id;
	char* raw_image_data_pointer;
	INT		m_nSizeX;		// width of video
	INT		m_nSizeY;		// height of video
	INT		color_mode;	// Y8/RGB16/RGB24/REG32
	INT		bits_per_pixel;// number of bits needed store one pixel
};

struct camera_list_deleter2
{
	void operator()(UEYE_CAMERA_LIST* list_ptr) const
	{
		delete[] ((BYTE*)list_ptr);
	}
};

#endif // UEYE_UI1490SEMGL_CAMERA_H
