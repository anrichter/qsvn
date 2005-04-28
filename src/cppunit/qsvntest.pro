LIBS += -lqttestrunner \
        -lcppunit
OBJECTS_DIR = .obj
TARGET = qsvnest
DESTDIR = bin
CONFIG += release \
          warn_on \
          thread
TEMPLATE = app
HEADERS += svnclienttests.h
SOURCES += main.cpp \
           svnclienttests.cpp
win32{
  OBJECTS_DIR = obj
}
