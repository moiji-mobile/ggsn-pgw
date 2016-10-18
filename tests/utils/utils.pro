QT += testlib
CONFIG += testcase
CONFIG += silent
CONFIG -= app_bundle
TARGET = test_utils

INCLUDEPATH += ../../src

SOURCES += \
	../../src/utils/logging.cc \
	utils_test.cc
