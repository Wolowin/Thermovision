#-------------------------------------------------
#
# Project created by QtCreator 2014-04-19T14:17:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Thermovision
TEMPLATE = app

include(Model/Model.pri)
include(Controller/Controller.pri)
include(View/View.pri)
include(Interface/Interface.pri)

SOURCES += \
	main.cpp

HEADERS  += \
	logdebug.h

FORMS    +=

OTHER_FILES += \
	Model/Model.pri \
	View/View.pri \
	Controller/Controller.pri \
	Interface/Interface.pri \
	Globals/zGlobals.pri
