#ifndef ABSTRACT_IMAGE_TRANSFORMATION_H
#define ABSTRACT_IMAGE_TRANSFORMATION_H

#include <QImage>

class abstract_image_transformation
{
public:
	abstract_image_transformation() {}
	virtual ~abstract_image_transformation() {}

	virtual QImage apply_transformation(QImage& transformed_image) = 0;

};

#endif // ABSTRACT_IMAGE_TRANSFORMATION_H
