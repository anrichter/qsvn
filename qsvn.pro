TEMPLATE     = app
CONFIG      += release \
               warn_on \
               thread

TARGET       = qsvn
DESTDIR      = bin

SOURCES     += main.cpp \
               qsvn.cpp \
               addworkingcopy.cpp
HEADERS     += qsvn.h \
               addworkingcopy.h
FORMS       += qsvndlg.ui \
               addworkingcopydlg.ui

MOC_DIR      = .moc
OBJECTS_DIR  = .obj

win32 {
    MOC_DIR      = moc
    OBJECTS_DIR  = obj
}
