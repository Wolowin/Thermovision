#include "Interface/Camera/camera_factory.h"
#include "ueye_ui1120semgl_camera.h"
#include "ueye_ui1490semgl_camera.h"
abstract_camera_interface *camera_factory::get_camera_object()
{
	return new UEYE_UI1490SEMGL_camera;
}
