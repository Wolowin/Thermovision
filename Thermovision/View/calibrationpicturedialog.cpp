#include "calibrationpicturedialog.h"
#include "ui_calibrationpicturedialog.h"

#include <iostream>
using namespace std;
CalibrationPictureDialog::CalibrationPictureDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CalibrationPictureDialog)
{
	ui->setupUi(this);
	drawable_qlabel = new DrawableQLabel(QImage(), this);
	ui->horizontalLayout_3->addWidget(drawable_qlabel);
}

CalibrationPictureDialog::~CalibrationPictureDialog()
{
	delete ui;
}

QImage CalibrationPictureDialog::get_selected_image_part()
{
	QRect AOI_rect = drawable_qlabel->get_selection_rectangle();

	if (default_AOI != AOI_rect && ui->checkBox_make_new_aoi->isChecked())
	{
		default_AOI = AOI_rect;
	}

	cout << "XXXXX AOI_rect.height()" << AOI_rect.height() << endl;
	QImage AOI_image_part =
			drawable_qlabel->get_calibration_picture().copy(AOI_rect);
	return AOI_image_part;
}

int CalibrationPictureDialog::get_current_temperature()
{
	return ui->spinBox_temperature->value();
}

void CalibrationPictureDialog::set_image(QImage the_image_to_set)
{
	QImage copy = the_image_to_set.copy();
	drawable_qlabel->put_picture_in_label(copy);
	drawable_qlabel->set_starting_selection_rectangle(default_AOI);
	drawable_qlabel->update();
}
