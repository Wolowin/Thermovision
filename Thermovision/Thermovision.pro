#-------------------------------------------------
#
# Project created by QtCreator 2014-04-19T14:17:24
#
#-------------------------------------------------

QT       += core gui
QT += gui-private

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Thermovision
TEMPLATE = app


include(Model/Model.pri)
include(Controller/Controller.pri)
include(View/View.pri)
include(Interface/Interface.pri)

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











#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../Qt/Qt5.2.1/5.2.1/mingw48_32/lib/ -lQt5Core
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../Qt/Qt5.2.1/5.2.1/mingw48_32/lib/ -lQt5Cored
#INCLUDEPATH += $$PWD/../../../../../../../Qt/Qt5.2.1/5.2.1/mingw48_32/include
#DEPENDPATH += $$PWD/../../../../../../../Qt/Qt5.2.1/5.2.1/mingw48_32/include
#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../../../Qt/Qt5.2.1/5.2.1/mingw48_32/lib/libQt5Core.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../../../Qt/Qt5.2.1/5.2.1/mingw48_32/lib/libQt5Cored.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../../../Qt/Qt5.2.1/5.2.1/mingw48_32/lib/Qt5Core.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../../../Qt/Qt5.2.1/5.2.1/mingw48_32/lib/Qt5Cored.lib


