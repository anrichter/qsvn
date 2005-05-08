TARGET = qsvntests
DESTDIR = bin
TEMPLATE = app
CONFIG += release \
          warn_on \
          thread

HEADERS += config.h \
           svnclienttests.h \
           ../svnclient.h

SOURCES += main.cpp \
           svnclienttests.cpp \
           ../svnclient.cpp

unix{
  OBJECTS_DIR = .obj
  MOC_DIR = .moc
  LIBS += -lsvn_client-1 \
          -lcppunit \
          -lqttestrunner
  INCLUDEPATH += /usr/include/subversion-1 \
                 /usr/include/apr-0
}

win32{
  OBJECTS_DIR = obj
  MOC_DIR = moc
  LIBS += -lcppunit \
          -lintl3_svn \
          -llibapr \
          -llibapriconv \
          -llibaprutil \
          -llibdb43 \
          -llibneon \
          -llibsvn_client-1 \
          -llibsvn_delta-1 \
          -llibsvn_diff-1 \
          -llibsvn_fs-1 \
          -llibsvn_fs_base-1 \
          -llibsvn_fs_fs-1 \
          -llibsvn_ra-1 \
          -llibsvn_ra_dav-1 \
          -llibsvn_ra_local-1 \
          -llibsvn_ra_svn-1 \
          -llibsvn_repos-1 \
          -llibsvn_subr-1 \
          -llibsvn_wc-1 \
          -lTestRunner \
          -lqttestrunner \
          -lxml \
          -lzlibstat
}
