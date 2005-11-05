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
          -lsvn_wc-1
}

win32 {
  MOC_DIR = moc
  UI_DIR = ui
  OBJECTS_DIR = obj

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
        forms\configure.ui \
        forms\qsvn.ui

HEADERS = addworkingcopy.h \
          config.h \
          configure.h \
          filelistitem.h \
          filelistmodel.h \
          qsvn.h \
          statustext.h \
          svnclient.h \
          workingcopyitem.h \
          workingcopymodel.h \
          svncpp/annotate_line.hpp \
          svncpp/apr.hpp \
          svncpp/check.hpp \
          svncpp/client.hpp \
          svncpp/context.hpp \
          svncpp/context_listener.hpp \
          svncpp/datetime.hpp \
          svncpp/dirent.hpp \
          svncpp/entry.hpp \
          svncpp/exception.hpp \
          svncpp/info_entry.hpp \
          svncpp/lock_entry.hpp \
          svncpp/log_entry.hpp \
          svncpp/path.hpp \
          svncpp/pool.hpp \
          svncpp/property.hpp \
          svncpp/revision.hpp \
          svncpp/status.hpp \
          svncpp/targets.hpp \
          svncpp/url.hpp \
          svncpp/version_check.hpp \
          svncpp/wc.hpp

SOURCES = addworkingcopy.cpp \
          config.cpp \
          configure.cpp \
          filelistitem.cpp \
          filelistmodel.cpp \
          main.cpp \
          qsvn.cpp \
          statustext.cpp \
          svnclient.cpp \
          workingcopyitem.cpp \
          workingcopymodel.cpp \
          svncpp/apr.cpp \
          svncpp/client_annotate.cpp \
          svncpp/client_cat.cpp \
          svncpp/client.cpp \
          svncpp/client_diff.cpp \
          svncpp/client_lock.cpp \
          svncpp/client_ls.cpp \
          svncpp/client_modify.cpp \
          svncpp/client_property.cpp \
          svncpp/client_status.cpp \
          svncpp/context.cpp \
          svncpp/datetime.cpp \
          svncpp/dirent.cpp \
          svncpp/entry.cpp \
          svncpp/exception.cpp \
          svncpp/info_entry.cpp \
          svncpp/lock_entry.cpp \
          svncpp/log_entry.cpp \
          svncpp/path.cpp \
          svncpp/pool.cpp \
          svncpp/property.cpp \
          svncpp/revision.cpp \
          svncpp/status.cpp \
          svncpp/targets.cpp \
          svncpp/url.cpp \
          svncpp/version_check.cpp \
          svncpp/wc.cpp
