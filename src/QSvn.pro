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

FORMS = Forms\QSvn.ui

HEADERS = FileListItem.h \
          FileListModel.h \
          QSvn.h \
          WorkingCopyItem.h \
          WorkingCopyModel.h

SOURCES = FileListItem.cpp \
          FileListModel.cpp \
          Main.cpp \
          QSvn.cpp \
          WorkingCopyItem.cpp \
          WorkingCopyModel.cpp
