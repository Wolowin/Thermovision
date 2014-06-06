#include "Interface/Camera/camera_factory.h"
#include "ueye_ui1120semgl_camera.h"

abstract_camera_interface camera_factory::get_camera_object()
{
	return new uEYE_UI1120SEMGL_camera;
}
