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

FORMS = Forms/AboutDlg.ui \
        Forms/AddWorkingCopyDlg.ui \
        Forms/CheckoutDlg.ui \
        Forms/ConfigureDlg.ui \
        Forms/FileSelectorDlg.ui \
        Forms/QSvnDlg.ui

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

IMAGES += Images/AddedFile.png \
          Images/ConflictFile.png \
          Images/File.png \
          Images/Folder.png \
          Images/MissingFile.png \
          Images/ModifiedFile.png \
          Images/UnknownFile.png \
          Images/UnknownFolder.png

win32{
  MOC_DIR = moc
  OBJECTS_DIR = obj
  UI_DIR = ui
}
