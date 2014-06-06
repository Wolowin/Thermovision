#-------------------------------------------------
#
# Project created by QtCreator 2014-04-19T14:17:24
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Thermovision
TEMPLATE = app


include(Model/Model.pri)
include(Controller/Controller.pri)
include(View/View.pri)
include(Interface/Interface.pri)
include(Camera/Camera.pri)

OBJECTS_DIR = obj
UI_DIR = ui
MOC_DIR = moc

SOURCES += main.cpp
QMAKE_CXXFLAGS += -std=c++11

HEADERS  += \
	log.h

INCLUDEPATH += $$PWD/../../../../../../../MinGW/msys/1.0/local/include
LIBS += -L$$PWD/../../../../../../../MinGW/msys/1.0/local/lib/ -lboost_thread-mgw48-mt-1_54
LIBS += -L$$PWD/../../../../../../../MinGW/msys/1.0/local/lib/ -lboost_system-mgw48-mt-1_54

LIBS += -L$$PWD/../../../../../../../Qt/Qt5.2.1/5.2.1/mingw48_32/lib/

#for uEye functions
INCLUDEPATH += $$PWD/../uEye/include
LIBS += -L$$PWD/../uEye/lib/ -luEye_api
