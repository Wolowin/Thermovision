#ifndef IMAGE_SAVER_H
#define IMAGE_SAVER_H

#include <QImage>
#include "Interface/calibration_parameters.h"

class image_saver
{
public:
	image_saver();
	void save_image(QImage image_to_save, calibration_parameters the_parameters, int temperature);
};

#endif // IMAGE_SAVER_H
