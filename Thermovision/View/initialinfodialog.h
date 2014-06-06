#ifndef INITIALINFODIALOG_H
#define INITIALINFODIALOG_H

#include <QDialog>
#include "Interface/calibration_parameters.h"

namespace Ui {
class InitialInfoDialog;
}

class InitialInfoDialog : public QDialog
{
	Q_OBJECT

public:
	explicit InitialInfoDialog(QWidget *parent = 0);
	~InitialInfoDialog();

	calibration_parameters get_given_calibration_parameters();
private:
	Ui::InitialInfoDialog *ui;
};

#endif // INITIALINFODIALOG_H
