TEMPLATE = app
TARGET = QSvn
DESTDIR = bin

unix:MOC_DIR = .moc
win32:MOC_DIR = moc
unix:UI_DIR = .ui
win32:UI_DIR = ui
unix:OBJECTS_DIR = .obj
win32:OBJECTS_DIR = obj

CONFIG += qt

RESOURCES = QSvn.qrc

FORMS = Forms\AddWorkingCopy.ui \
        Forms\Configure.ui \
        Forms\QSvn.ui

HEADERS = AddWorkingCopy.h \
          Config.h \
          Configure.h \
          FileListItem.h \
          FileListModel.h \
          QSvn.h \
          StatusText.h \
          SvnWrapper.h \
          WorkingCopyItem.h \
          WorkingCopyModel.h

SOURCES = AddWorkingCopy.cpp \
          Config.cpp \
          Configure.cpp \
          FileListItem.cpp \
          FileListModel.cpp \
          Main.cpp \
          QSvn.cpp \
          StatusText.cpp \
          SvnWrapper.cpp \
          WorkingCopyItem.cpp \
          WorkingCopyModel.cpp
