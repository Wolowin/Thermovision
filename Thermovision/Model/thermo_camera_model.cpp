#include "thermo_camera_model.h"
#include "../log.h"
#include <QDomDocument>



thermo_camera_model::thermo_camera_model():
	operating_camera_handle(starting_camera_handle),
	sensor_info_ptr(new SENSORINFO)
{
	get_connected_cameras_infos();
	initialize_camera();
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
	log_debug("Camera preparations finished!");
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

char *thermo_camera_model::get_data_pointer()
{
	return reinterpret_cast<char*>(raw_image_data_pointer);
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

void thermo_camera_model::get_sensor_infos()
{
	if (is_GetSensorInfo(operating_camera_handle, sensor_info_ptr.get()) != IS_SUCCESS){
		log_error("Failed to read sensor info");
	} else {
		log_debug("Sensor info read properly");
	}
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

void thermo_camera_model::allocate_memory_for_image()
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

void thermo_camera_model::image_capture()
{
	is_CaptureVideo(operating_camera_handle, IS_WAIT);
	log_debug("Capturing image");
}

void thermo_camera_model::run_calibration(calibration_parameters the_parameters)
{

	// przebieg kalibracji

	QDomDocument document("Calibration profiles");

		QDomElement first_profile = document.createElement( "profile_one" );
		first_profile.setAttribute( "name", "spawanie" );
		QDomElement two_profile = document.createElement( "profile_two" );
		two_profile.setAttribute( "name", "lutowanie" );
		QDomElement three_profile = document.createElement( "profile_three" );
		three_profile.setAttribute( "name", "hartowanie" );


		QDomElement first_parameters = document.createElement( "Parameters" );
		first_parameters.setAttribute( "Camera_model", "some_model" );
		first_parameters.setAttribute( "Filter_model", "some_filter_model" );
		first_parameters.setAttribute( "jjytun", "wqer" );
		first_parameters.setAttribute( "untrnu", "dsa" );
		first_parameters.setAttribute( "ytitr", "gsdadel" );
		first_parameters.setAttribute( "uyloyumodel", "somhdhfddel" );
		first_parameters.setAttribute( "iyhu", "somejfdodel" );

		QDomText first_text = document.createTextNode( "profile description" );

		document.appendChild( first_profile );
		document.appendChild( two_profile );
		document.appendChild( three_profile );

		first_profile.appendChild( first_parameters );
		first_profile.appendChild( first_text );


		QFile file( "simple.xml" );
		if( !file.open( QIODevice::WriteOnly | QIODevice::Text ) )
		{
		qDebug( "Failed to open file for writing." );
		return;
		}
		QTextStream stream( &file );
		stream << document.toString();
		file.close();
}
