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

FORMS = Forms\Configure.ui \
        Forms\QSvn.ui

HEADERS = Config.h \
          Configure.h \
          FileListItem.h \
          FileListModel.h \
          QSvn.h \
          StatusText.h \
          WorkingCopyItem.h \
          WorkingCopyModel.h

SOURCES = Config.cpp \
          Configure.cpp \
          FileListItem.cpp \
          FileListModel.cpp \
          Main.cpp \
          QSvn.cpp \
          StatusText.cpp \
          WorkingCopyItem.cpp \
          WorkingCopyModel.cpp
