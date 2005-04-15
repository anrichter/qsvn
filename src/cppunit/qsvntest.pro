LIBS += -lqttestrunner \
        -lcppunit
OBJECTS_DIR = .obj
TARGET = qsvnest
DESTDIR = bin
CONFIG += release \
          warn_on \
          thread
TEMPLATE = app
HEADERS += svnclient.h
SOURCES += main.cpp \
           svnclient.cpp
win32{
  OBJECTS_DIR = obj
}
