TARGET = qsvntests
DESTDIR = bin
TEMPLATE = app
CONFIG += release \
          warn_on \
          thread

LIBS += -lcppunit \
        -lsvn_client-1

HEADERS += config.h \
           svnclienttests.h \
           ../svnclient.h

SOURCES += main.cpp \
           svnclienttests.cpp \
           ../svnclient.cpp

OBJECTS_DIR = .obj

unix{
  LIBS += -lqttestrunner
  INCLUDEPATH += /usr/include/subversion-1 \
                 /usr/include/apr-0
}

win32{
  OBJECTS_DIR = obj
}
