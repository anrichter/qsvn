MOC_DIR = .moc
UI_DIR = .ui
OBJECTS_DIR = .obj
TARGET = QSvn
DESTDIR = bin
CONFIG += release \
          warn_on \
          qt \
          thread
TEMPLATE = app

FORMS = Forms/aboutdlg.ui \
        Forms/addworkingcopydlg.ui \
        Forms/checkoutdlg.ui \
        Forms/configuredlg.ui \
        Forms/fileselectordlg.ui \
        Forms/qsvndlg.ui

HEADERS = AddWorkingCopy.h \
          Checkout.h \
          Config.h \
          Configure.h \
          FileList.h \
          FileListItem.h \
          FileSelector.h \
          QSvn.h \
          StatusText.h \
          SvnWrapper.h \
          WorkingCopy.h \
          WorkingCopyItem.h

SOURCES = AddWorkingCopy.cpp \
          Checkout.cpp \
          Config.cpp \
          Configure.cpp \
          FileList.cpp \
          FileListItem.cpp \
          FileSelector.cpp \
          Main.cpp \
          QSvn.cpp \
          StatusText.cpp \
          SvnWrapper.cpp \
          WorkingCopy.cpp \
          WorkingCopyItem.cpp

IMAGES += Images/addedfile.png \
          Images/conflictfile.png \
          Images/file.png \
          Images/folder.png \
          Images/missingfile.png \
          Images/modifiedfile.png \
          Images/unknownfile.png \
          Images/unknownfolder.png

win32{
  MOC_DIR = moc
  OBJECTS_DIR = obj
  UI_DIR = ui
}
