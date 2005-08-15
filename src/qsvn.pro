TEMPLATE = app
TARGET = qsvn
DESTDIR = bin

unix:MOC_DIR = .moc
win32:MOC_DIR = moc
unix:UI_DIR = .ui
win32:UI_DIR = ui
unix:OBJECTS_DIR = .obj
win32:OBJECTS_DIR = obj

CONFIG += qt

RESOURCES = qsvn.qrc

FORMS = forms\AddWorkingCopy.ui \
        forms\Configure.ui \
        forms\QSvn.ui

HEADERS = addworkingcopy.h \
          config.h \
          configure.h \
          filelistitem.h \
          filelistmodel.h \
          qsvn.h \
          statustext.h \
          svnwrapper.h \
          workingcopyitem.h \
          workingcopymodel.h

SOURCES = addworkingcopy.cpp \
          config.cpp \
          configure.cpp \
          filelistitem.cpp \
          filelistmodel.cpp \
          main.cpp \
          qsvn.cpp \
          statustext.cpp \
          svnwrapper.cpp \
          workingcopyitem.cpp \
          workingcopymodel.cpp
