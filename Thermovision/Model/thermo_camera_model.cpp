#include "thermo_camera_model.h"
#include "../log.h"




thermo_camera_model::thermo_camera_model():
	operating_camera_handle(starting_camera_handle),
	sensor_info_ptr(new SENSORINFO)
{
	get_connected_cameras_infos();
	initialize_camera();
	get_sensor_infos();
	// setup the color depth to the current windows setting
	is_GetColorDepth (operating_camera_handle, &m_nBitsPerPixel, &m_nColorMode);
	is_SetColorMode (operating_camera_handle, m_nColorMode);

	// Check if the camera supports an arbitrary AOI
	// Only the ueye xs does not support an arbitrary AOI
	GetMaxImageSize();

//	double nCaps = 0;
//	INT nRet = is_Exposure(operating_camera_handle, IS_EXPOSURE_CMD_GET_EXPOSURE, &nCaps, sizeof(nCaps));
//	cout << "nCaps: " << nCaps << endl;
//cout << "nRet: " << (int)nRet << endl;
//	nCaps = 100;
//	cout << "nCaps: " << nCaps << endl;
//	 nRet = is_Exposure(operating_camera_handle, IS_EXPOSURE_CMD_SET_EXPOSURE, &nCaps, sizeof(nCaps));
//	cout << "nCaps: " << nCaps << endl;
//	cout << "nRet: " << (int)nRet << endl;

//	 nRet = is_Exposure(operating_camera_handle, IS_EXPOSURE_CMD_GET_EXPOSURE, &nCaps, sizeof(nCaps));
//	cout << "nCaps: " << nCaps << endl;
//	cout << "nRet: " << (int)nRet << endl;
}

thermo_camera_model::~thermo_camera_model()
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

void thermo_camera_model::get_connected_cameras_infos()
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

void thermo_camera_model::qwe()
{

Sleep(3000);
allocate_memory_for_image();
	is_SetDisplayMode (operating_camera_handle, IS_SET_DM_DIB);

	is_SetImageMem (operating_camera_handle, raw_image_data_pointer, image_memory_id);

	is_CaptureVideo(operating_camera_handle, IS_WAIT);
	log_debug("Rendering image");
//	is_RenderBitmap (operating_camera_handle, image_memory_id, test, IS_RENDER_FIT_TO_WINDOW);
//	log_debug("Image Rendered");

//	is_SetImageMem (operating_camera_handle, raw_image_data_pointer, image_memory_id);
//	is_FreezeVideo(operating_camera_handle, IS_WAIT);
//	log_debug("Rendering image2");
//	is_RenderBitmap (operating_camera_handle, image_memory_id, test, IS_RENDER_FIT_TO_WINDOW);
//	log_debug("Image Rendered2");

//	auto_status = is_GetAutoInfo ( operating_camera_handle,  &auto_inf);
//	cout << "auto_status: " << auto_status << endl;
//cout << "auto_infAutoAbility.: " << std::hex<< auto_inf.AutoAbility  << endl;
//cout << "auto_inf.sBrightCtrlStatus.curController : "  << std::hex<<auto_inf.sBrightCtrlStatus.curController << endl;
}

void thermo_camera_model::GetMaxImageSize()
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

void thermo_camera_model::alloc_mem_for_camera_list()
{
	boost::shared_ptr<UEYE_CAMERA_LIST> tmp(
				(UEYE_CAMERA_LIST*) new BYTE [sizeof (DWORD) + number_of_cameras * sizeof (UEYE_CAMERA_INFO)],
			camera_list_deleter());
	camera_list_ptr = tmp;
}

void thermo_camera_model::get_camera_list()
{
	if (is_GetCameraList(camera_list_ptr.get()) != IS_SUCCESS) {
		log_error("Failed to GetCameraList");
	} else {
		log_debug("Camera list received succesfully");
	}
}

void thermo_camera_model::get_sensor_infos()
{
	if (is_GetSensorInfo(operating_camera_handle, sensor_info_ptr.get()) != IS_SUCCESS){
		log_error("Failed to read sensor info");
	} else {
		log_debug("Sensor info read properly");
	}
}

void thermo_camera_model::initialize_camera()
{
	INT nRet = is_InitCamera (&operating_camera_handle, NULL);
	if (nRet != IS_SUCCESS) {
		check_if_firmware_update_needed(nRet);
	} else {
		log_debug("Camera is initialized");
	}
}
void thermo_camera_model::check_if_firmware_update_needed(INT nRet)
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

void thermo_camera_model::allocate_memory_for_image()
{
	if (is_AllocImageMem(operating_camera_handle,
					 m_nSizeX,
					 m_nSizeY,
					 m_nBitsPerPixel,
					 &raw_image_data_pointer,
					 &image_memory_id) != IS_SUCCESS){
		log_error("Failed to allocate memory for image");
	} else {
		log_debug("Memory for image allocated");
	}
}

INT thermo_camera_model::convert_to_bits_per_pixel(char color_mode)
{
	if (sensor_info_ptr->nColorMode == color_mode)
		return 8;
	if (sensor_info_ptr->nColorMode == color_mode)
		return 24;
	if (sensor_info_ptr->nColorMode == color_mode)
		return 24;
	if (sensor_info_ptr->nColorMode == color_mode)
		return 24;
	if (sensor_info_ptr->nColorMode == color_mode)
		log_error("Sensors ColorMode Invalid");
}
