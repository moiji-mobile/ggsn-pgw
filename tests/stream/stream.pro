QT += testlib
CONFIG += testcase
CONFIG += silent
CONFIG -= app_bundle
TARGET = test_stream

INCLUDEPATH += ../../src

SOURCES += \
	stream_test.cc
