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
		void hide_profile_details_labels();
public slots:
	void show_calibration_toolbar();
	void start_measurement();
	void stop_measurement();
	void show_profile_details(QString profile_name);
	void create_new_profile();
	void calibration_photo_capture_initialized();
	void update_profiles_list();

private:
	boost::scoped_ptr<abstract_thermo_camera_model> the_model;
	MainWindow main_window;

	void show_legend_bar_label();
	void change_profile_details_visibility(bool show);
};

#endif // QT_THERMO_VISION_CONTROLLER_H
