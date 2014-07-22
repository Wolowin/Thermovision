#ifndef CALIBRATIONPICTUREDIALOG_H
#define CALIBRATIONPICTUREDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QImage>
#include <QRect>
#include "drawableqlabel.h"
namespace Ui {
class CalibrationPictureDialog;
}

class CalibrationPictureDialog : public QDialog
{
	Q_OBJECT

public:
	explicit CalibrationPictureDialog( QWidget *parent = 0);
	~CalibrationPictureDialog();

	QImage get_selected_image_part();
	int get_current_temperature();
	void set_image(QImage the_image_to_set);
private:
	Ui::CalibrationPictureDialog *ui;
	DrawableQLabel *drawable_qlabel;
	QRect default_AOI;
};

#endif // CALIBRATIONPICTUREDIALOG_H
