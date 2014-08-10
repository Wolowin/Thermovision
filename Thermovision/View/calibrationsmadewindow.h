#ifndef CALIBRATIONSMADEWINDOW_H
#define CALIBRATIONSMADEWINDOW_H

#include <QDialog>

#include "Interface/gain_exposure_temp_map.h"

namespace Ui {
class CalibrationsMadeWindow;
}

class CalibrationsMadeWindow : public QDialog
{
	Q_OBJECT

public:
	explicit CalibrationsMadeWindow(gain_exposure_temp_map the_map, QWidget *parent = 0);
	~CalibrationsMadeWindow();

private:
	Ui::CalibrationsMadeWindow *ui;

	gain_exposure_temp_map copied_map;
};

#endif // CALIBRATIONSMADEWINDOW_H
