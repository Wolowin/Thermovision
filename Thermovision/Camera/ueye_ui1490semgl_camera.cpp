#include "UEYE_UI1490SEMGL_camera.h"
#include "log.h"

UEYE_UI1490SEMGL_camera::UEYE_UI1490SEMGL_camera():
	operating_camera_handle(starting_camera_handle),
	sensor_info_ptr(new SENSORINFO)
{

}

void UEYE_UI1490SEMGL_camera::initialize_camera()
{
	get_connected_cameras_infos();
	init_camera();
	get_sensor_infos();
	// setup the color depth to the current windows setting
	is_GetColorDepth (operating_camera_handle, &bits_per_pixel, &color_mode);
	is_SetColorMode (operating_camera_handle, color_mode);

	// Check if the camera supports an arbitrary AOI
	// Only the ueye xs does not support an arbitrary AOI
	GetMaxImageSize();

	Sleep(3000);
	allocate_memory_for_image();
	is_SetDisplayMode (operating_camera_handle, IS_SET_DM_DIB);
	is_SetImageMem (operating_camera_handle, raw_image_data_pointer, image_memory_id);


	log_debug("ZACZYNAMY ZABAWE!");

	UEYE_AUTO_INFO auto_info;
	is_GetAutoInfo(operating_camera_handle, &auto_info);
	cout << "AutoAbility: " << auto_info.AutoAbility << endl;
	cout << "AUTO_BRIGHT_STATUS.curValue: " << auto_info.sBrightCtrlStatus.curValue << endl;
	cout << "AUTO_BRIGHT_STATUS.curController: " << auto_info.sBrightCtrlStatus.curController << endl;
	double pva11 = 0;   //disable autogain
	double pva12 = 0;

	INT ret = is_SetAutoParameter(operating_camera_handle, IS_SET_ENABLE_AUTO_GAIN, &pva11, &pva12);
	cout << "1 ret " <<  ret<< endl;
	if (ret == IS_SUCCESS)    //disable autogain
		cout << "Pierwsze ok" << endl;

	if (is_SetAutoParameter(operating_camera_handle, IS_SET_ENABLE_AUTO_SENSOR_GAIN, &pva11, &pva12) == IS_SUCCESS)    //disable sensor autogain
		cout << "Drugie ok" << endl;

	log_debug("KONCZYMY ZABAWE!");
	log_debug("Camera preparations finished!");
}

void UEYE_UI1490SEMGL_camera::deinitialize_camera()
{
	if (is_FreeImageMem(operating_camera_handle, raw_image_data_pointer, image_memory_id) != IS_SUCCESS){
		log_error("Failed to release the image memory");
	} else {
		log_debug("Image memory released");
	}

	if (is_ExitCamera(operating_camera_handle) != IS_SUCCESS){
		log_error("Failed to close the camera");
	} else {
		log_debug("Camera is properly closed");
	}
}

char *UEYE_UI1490SEMGL_camera::get_data_pointer()
{
	return reinterpret_cast<char*>(raw_image_data_pointer);
}

void UEYE_UI1490SEMGL_camera::capture_picture()
{
	is_FreezeVideo(operating_camera_handle, IS_WAIT);
}

void UEYE_UI1490SEMGL_camera::start_live_video()
{
	is_CaptureVideo(operating_camera_handle, IS_WAIT);
	log_debug("Capturing image");
}

void UEYE_UI1490SEMGL_camera::stop_live_video()
{
	is_StopLiveVideo(operating_camera_handle, IS_WAIT);
	log_debug("Stopping live video");
}

int UEYE_UI1490SEMGL_camera::set_gain_percent(int gain_percent)
{
	log_debug("Setting gain...");
	INT return_status = is_SetHardwareGain(operating_camera_handle, gain_percent, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
	cout << "set_gain_percent return_status: " << return_status << endl;

	return 0;
}

int UEYE_UI1490SEMGL_camera::set_exposure_time_in_ms(int exposure_time_ms)
{
	double tmp;
	is_Exposure (operating_camera_handle, IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE_MIN, &tmp, sizeof(tmp));
	cout << "min: " << tmp << endl;
	is_Exposure (operating_camera_handle, IS_EXPOSURE_CMD_GET_EXPOSURE_RANGE_MAX, &tmp, sizeof(tmp));
	cout << "max: " << tmp << endl;

	UINT nRange[3];
	INT nRet = is_PixelClock(operating_camera_handle, IS_PIXELCLOCK_CMD_GET_RANGE, (void*)nRange, sizeof(nRange));
	cout << "Min pixel clock range: " << nRange[0] << "MHz" << endl;
	cout << "Max pixel clock range: " << nRange[1] << "MHz" << endl;
	double exposure_time = exposure_time_ms;
	INT return_status = is_Exposure (operating_camera_handle, IS_EXPOSURE_CMD_SET_EXPOSURE, &exposure_time, sizeof(exposure_time));
	log_debug("Setting exposure time.");

	cout << "return_status: " << return_status << " z exposure time: " << exposure_time << "ms"<< endl;
	return return_status;
}

int UEYE_UI1490SEMGL_camera::get_gain_percent(int &gain_percent)
{
	log_debug("Getting gain setting...");
	gain_percent = is_SetHardwareGain(operating_camera_handle, IS_GET_MASTER_GAIN, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER, IS_IGNORE_PARAMETER);
	cout << "get_gain_percent value: " << gain_percent << endl;
	return 0;
}

int UEYE_UI1490SEMGL_camera::get_exposure_time_in_ms(int &exposure_time_ms)
{
	log_debug("Getting exposure time...");
	double exposure_time;
	INT return_status = is_Exposure (operating_camera_handle, IS_EXPOSURE_CMD_GET_EXPOSURE, (void*)&exposure_time, sizeof(exposure_time));
	exposure_time_ms = exposure_time;
	return return_status;
}

int UEYE_UI1490SEMGL_camera::get_image_size_x()
{
	return m_nSizeX;
}

int UEYE_UI1490SEMGL_camera::get_image_size_y()
{
	return m_nSizeY;
}

void UEYE_UI1490SEMGL_camera::get_connected_cameras_infos()
{
	if( is_GetNumberOfCameras( &number_of_cameras ) == IS_SUCCESS) {
		if( number_of_cameras != 1 ) {
			log_error("Not supported number of cameras (either 0 or higher than 1)");
		} else {
			alloc_mem_for_camera_list();
			get_camera_list();
		}
	}
}

void UEYE_UI1490SEMGL_camera::alloc_mem_for_camera_list()
{
	boost::shared_ptr<UEYE_CAMERA_LIST> tmp(
				(UEYE_CAMERA_LIST*) new BYTE [sizeof (DWORD) + number_of_cameras * sizeof (UEYE_CAMERA_INFO)],
			camera_list_deleter2());
	camera_list_ptr = tmp;
}

void UEYE_UI1490SEMGL_camera::get_camera_list()
{
	if (is_GetCameraList(camera_list_ptr.get()) != IS_SUCCESS) {
		log_error("Failed to GetCameraList");
	} else {
		log_debug("Camera list received succesfully");
	}
}

void UEYE_UI1490SEMGL_camera::init_camera()
{
	INT nRet = is_InitCamera (&operating_camera_handle, NULL);
	if (nRet != IS_SUCCESS) {
		check_if_firmware_update_needed(nRet);
	} else {
		log_debug("Camera is initialized");
	}
}

void UEYE_UI1490SEMGL_camera::check_if_firmware_update_needed(INT nRet)
{
	//Check if GigE uEye SE needs a new starter firmware
	if (nRet == IS_STARTER_FW_UPLOAD_NEEDED)
	{
		//Calculate time needed for updating the starter firmware
		INT nTime;
		is_GetDuration (operating_camera_handle, IS_SE_STARTER_FW_UPLOAD, &nTime);
		/*
	   e.g. have progress bar displayed in separate thread
	  */

		//Upload new starter firmware during initialization
		operating_camera_handle =  operating_camera_handle | IS_ALLOW_STARTER_FW_UPLOAD;
		nRet = is_InitCamera (&operating_camera_handle, NULL);

		/*
		end progress bar
	   */
	} else {
		log_error("Cant initialize camera");
	}
}

void UEYE_UI1490SEMGL_camera::get_sensor_infos()
{
	if (is_GetSensorInfo(operating_camera_handle, sensor_info_ptr.get()) != IS_SUCCESS){
		log_error("Failed to read sensor info");
	} else {
		log_debug("Sensor info read properly");
	}
}

void UEYE_UI1490SEMGL_camera::GetMaxImageSize()
{
	INT nAOISupported = 0;
	BOOL bAOISupported = TRUE;
	if (is_ImageFormat(operating_camera_handle,
					   IMGFRMT_CMD_GET_ARBITRARY_AOI_SUPPORTED,
					   (void*)&nAOISupported,
					   sizeof(nAOISupported)) == IS_SUCCESS)
	{
		bAOISupported = (nAOISupported != 0);
	}

	if (bAOISupported)
	{
		// All other sensors
		// Get maximum image size
		m_nSizeX = sensor_info_ptr->nMaxWidth;
		m_nSizeY = sensor_info_ptr->nMaxHeight;
	}
	else
	{
		// Only ueye xs
		// Get image size of the current format
		IS_SIZE_2D imageSize;
		is_AOI(operating_camera_handle, IS_AOI_IMAGE_GET_SIZE, (void*)&imageSize, sizeof(imageSize));

		m_nSizeX = imageSize.s32Width;
		m_nSizeY = imageSize.s32Height;
	}
}

void UEYE_UI1490SEMGL_camera::allocate_memory_for_image()
{
	if (is_AllocImageMem(operating_camera_handle,
					 m_nSizeX,
					 m_nSizeY,
					 bits_per_pixel,
					 &raw_image_data_pointer,
					 &image_memory_id) != IS_SUCCESS){
		log_error("Failed to allocate memory for image");
	} else {
		log_debug("Memory for image allocated");
	}
}
