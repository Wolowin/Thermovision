#include <QApplication>
#include "Controller/qt_thermo_vision_controller.h"

using namespace std;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	qt_thermo_vision_controller the_controller;
the_controller.run();
	return a.exec();
}
