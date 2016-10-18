QT += testlib
CONFIG += testcase
CONFIG += silent
CONFIG -= app_bundle
TARGET = test_statsd

INCLUDEPATH += ../../src

SOURCES += \
	../../src/utils/logging.cc \
	../../src/statsd/statsd.cc \
	statsd_test.cc
