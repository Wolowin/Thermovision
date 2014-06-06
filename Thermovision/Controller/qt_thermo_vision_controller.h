#ifndef QT_THERMO_VISION_CONTROLLER_H
#define QT_THERMO_VISION_CONTROLLER_H

#include "boost/scoped_ptr.hpp"

#include "../View/mainwindow.h"
#include "../Interface/Model/abstract_thermo_camera_model.h"

class qt_thermo_vision_controller: public QObject
{
	Q_OBJECT;
public:
	qt_thermo_vision_controller();

	void run();
public slots:
	void calibration_requested();

private:
	boost::scoped_ptr<abstract_thermo_camera_model> the_model;
	MainWindow w;

};

#endif // QT_THERMO_VISION_CONTROLLER_H
