#ifndef PROFILEPICKINGDIALOG_H
#define PROFILEPICKINGDIALOG_H

#include <QDialog>
#include "Interface/calibration_parameters.h"

namespace Ui {
class ProfilePickingDialog;
}

class ProfilePickingDialog : public QDialog
{
	Q_OBJECT

public:
	explicit ProfilePickingDialog(QWidget *parent = 0);
	~ProfilePickingDialog();
	calibration_parameters get_picked_profile_parameters();
private:
	Ui::ProfilePickingDialog *ui;
	std::vector <calibration_parameters> parameters_vector;
};

#endif // PROFILEPICKINGDIALOG_H
