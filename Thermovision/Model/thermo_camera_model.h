#ifndef THERMO_CAMERA_MODEL_H
#define THERMO_CAMERA_MODEL_H

#include "../Interface/Model/abstract_thermo_camera_model.h"


#include <uEye.h>
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

class thermo_camera_model : public abstract_thermo_camera_model
{
public:
	thermo_camera_model();
	virtual ~thermo_camera_model();

	virtual char* get_data_pointer();
	virtual void image_capture();

	virtual void run_calibration(calibration_parameters the_parameters);
private:
	void get_connected_cameras_infos();
	void alloc_mem_for_camera_list();
	void get_camera_list();
	void get_sensor_infos();
	void initialize_camera();
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


struct camera_list_deleter
{
	void operator()(UEYE_CAMERA_LIST* list_ptr) const
	{
		delete[] ((BYTE*)list_ptr);
	}
};

#endif // THERMO_CAMERA_MODEL_H
