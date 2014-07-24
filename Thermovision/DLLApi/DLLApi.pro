TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

HEADERS += \
	base.h

DEFINES -= UNICODE
DEFINES += _MBCS
