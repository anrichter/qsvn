TEMPLATE = app
TARGET = qsvn
DESTDIR = bin

unix {
  MOC_DIR = .moc
  UI_DIR = .ui
  OBJECTS_DIR = .obj

  INCLUDEPATH += /usr/include/subversion-1 \
                 /usr/include/apr-0

  LIBS += -lsvn_client-1 \
          -lsvn_delta-1 \
          -lsvn_diff-1 \
          -lsvn_fs-1 \
          -lsvn_ra-1 \
          -lsvn_repos-1 \
          -lsvn_subr-1 \
          -lsvn_wc-1 \
          -lapr-0 \
          -laprutil-0
}

win32 {
  CONFIG -= flat
  MOC_DIR = moc
  UI_DIR = ui
  OBJECTS_DIR = obj
  RC_FILE = qsvn.rc

  LIBS += -lintl3_svn \
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
          -lxml \
          -lzlibstat  \
          -lkernel32 \
          -luser32 \
          -lgdi32 \
          -lwinspool \
          -lcomdlg32 \
          -ladvapi32 \
          -lshell32 \
          -lole32 \
          -loleaut32 \
          -luuid \
          -lwsock32
}

CONFIG += qt

RESOURCES = qsvn.qrc

FORMS = forms\addworkingcopy.ui \
        forms\checkout.ui \
        forms\configure.ui \
        forms\fileselector.ui \
        forms\login.ui \
        forms\qsvn.ui

HEADERS = addworkingcopy.h \
          checkout.h \
          config.h \
          configure.h \
          filelistitem.h \
          filelistmodel.h \
          fileselector.h \
          listener.h \
          login.h \
          qsvn.h \
          statustext.h \
          svnclient.h \
          workingcopyitem.h \
          workingcopymodel.h \
          svnqt/annotate_line.hpp \
          svnqt/apr.hpp \
          svnqt/check.hpp \
          svnqt/client.hpp \
          svnqt/client_impl.hpp \
          svnqt/context.hpp \
          svnqt/context_listener.hpp \
          svnqt/datetime.hpp \
          svnqt/dirent.hpp \
          svnqt/entry.hpp \
          svnqt/exception.hpp \
          svnqt/info_entry.hpp \
          svnqt/lock_entry.hpp \
          svnqt/log_entry.hpp \
          svnqt/path.hpp \
          svnqt/pool.hpp \
          svnqt/property.hpp \
          svnqt/revision.hpp \
          svnqt/status.hpp \
          svnqt/targets.hpp \
          svnqt/url.hpp \
          svnqt/version_check.hpp \
          svnqt/wc.hpp

SOURCES = addworkingcopy.cpp \
          checkout.cpp \
          config.cpp \
          configure.cpp \
          filelistitem.cpp \
          filelistmodel.cpp \
          fileselector.cpp \
          listener.cpp \
          login.cpp \
          main.cpp \
          qsvn.cpp \
          statustext.cpp \
          svnclient.cpp \
          workingcopyitem.cpp \
          workingcopymodel.cpp \
          svnqt/apr.cpp \
          svnqt/client_annotate.cpp \
          svnqt/client_cat.cpp \
          svnqt/client.cpp \
          svnqt/client_diff.cpp \
          svnqt/client_impl.cpp \
          svnqt/client_lock.cpp \
          svnqt/client_ls.cpp \
          svnqt/client_modify.cpp \
          svnqt/client_property.cpp \
          svnqt/client_status.cpp \
          svnqt/context.cpp \
          svnqt/datetime.cpp \
          svnqt/dirent.cpp \
          svnqt/entry.cpp \
          svnqt/exception.cpp \
          svnqt/info_entry.cpp \
          svnqt/lock_entry.cpp \
          svnqt/log_entry.cpp \
          svnqt/path.cpp \
          svnqt/pool.cpp \
          svnqt/property.cpp \
          svnqt/revision.cpp \
          svnqt/status.cpp \
          svnqt/targets.cpp \
          svnqt/url.cpp \
          svnqt/version_check.cpp \
          svnqt/wc.cpp
