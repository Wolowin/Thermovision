#-------------------------------------------------
#
# Project created by QtCreator 2014-06-26T13:06:01
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UT_Thermovision
TEMPLATE = app


SOURCES += \

HEADERS  += \


QMAKE_CXXFLAGS += -std=c++11

include(Model/Model.pri)
include(Tests/Tests.pri)
include(Interface/Interface.pri)


win32: LIBS += -L$$PWD/../../../../../../../MinGW/msys/1.0/local/lib/ -lgmock
win32: LIBS += -L$$PWD/../../../../../../../MinGW/msys/1.0/local/lib/ -lgtest_main

INCLUDEPATH += $$PWD/../../../../../../../MinGW/msys/1.0/local/include
DEPENDPATH += $$PWD/../../../../../../../MinGW/msys/1.0/local/include

LIBS += -L$$PWD/../../../../../../../Qt/Qt5.2.1/5.2.1/mingw48_32/lib/


#DIRTY HACK !!!!!!!!!!!!!
#for factory function in model to compile
include(Camera/Camera.pri)
#for camera code to compile:
INCLUDEPATH += $$PWD/../uEye/include
LIBS += -L$$PWD/../uEye/lib/ -luEye_api
