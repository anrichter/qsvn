TEMPLATE     = app
CONFIG      += release \
               warn_on \
               thread

TARGET       = qsvn
DESTDIR      = bin

SOURCES     += main.cpp \
               qsvn.cpp
HEADERS     += qsvn.h
FORMS       += qsvndlg.ui

MOC_DIR      = .moc
OBJECTS_DIR  = .obj
