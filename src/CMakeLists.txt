SUBDIRS(svnqt/buildqt4)

FILE(GLOB uifiles forms/*.ui)
FILE(GLOB srces *.cpp)
FILE(GLOB headrs *.h)

QT4_WRAP_UI(ui_h ${uifiles})
QT4_AUTOMOC(${srces})
QT4_ADD_RESOURCES(resfiles qsvn.qrc)

ADD_EXECUTABLE(qsvn ${srces} ${ui_h} ${mocfiles} ${resfiles})

SET_TARGET_PROPERTIES(qsvn PROPERTIES LINK_FLAGS "${ALL_LINKFLAGS}")
target_link_libraries(qsvn svnqt-qt4 ${QT_LIBRARIES})
