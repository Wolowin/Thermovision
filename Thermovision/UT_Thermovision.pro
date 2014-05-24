TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

include(Model/Model.pri)
include(Tests/Tests.pri)

SOURCES +=


win32: LIBS += -L$$PWD/../../../../../../../MinGW/msys/1.0/local/lib/ -lgmock
win32: LIBS += -L$$PWD/../../../../../../../MinGW/msys/1.0/local/lib/ -lgtest_main

INCLUDEPATH += $$PWD/../../../../../../../MinGW/msys/1.0/local/include
DEPENDPATH += $$PWD/../../../../../../../MinGW/msys/1.0/local/include






