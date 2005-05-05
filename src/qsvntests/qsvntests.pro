LIBS += -lqttestrunner \
        -lcppunit

OBJECTS_DIR = .obj
win32{
  OBJECTS_DIR = obj
}

TARGET = qsvnests

DESTDIR = bin

CONFIG += release \
          warn_on \
          thread

TEMPLATE = app

HEADERS += config.h \
           svnclienttests.h

SOURCES += main.cpp \
           svnclienttests.cpp


# QSvn files
HEADERS += ../svnclient.h

SOURCES += ../svnclient.cpp

#Subversion
LIBS += -lsvn_client-1
INCLUDEPATH += /usr/include/subversion-1
INCLUDEPATH += /usr/include/apr-0
