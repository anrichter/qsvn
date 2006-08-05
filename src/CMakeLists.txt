SUBDIRS(svnqt/buildqt4)

FILE(GLOB uifiles forms/*.ui)
foreach(uifile ${uifiles})
 FILE(RELATIVE_PATH sfile ${QSVN_SOURCE_DIR}/src ${uifile})
 SET(inuifiles ${inuifiles} ${sfile})
endforeach(uifile)

QT4_WRAP_UI(ui_SOURCES ${inuifiles})

FILE(GLOB srces *.cpp)
FILE(GLOB headrs *.h)

QT4_AUTOMOC(${srces})
QT4_ADD_RESOURCES(resfiles qsvn.qrc)

ADD_EXECUTABLE(qsvn ${srces} ${ui_SOURCES} ${mocfiles} ${resfiles})

SET_SOURCE_FILES_PROPERTIES(${srces}
	PROPERTIES
	OBJECT_DEPENDS "${ui_SOURCES}")

SET_TARGET_PROPERTIES(qsvn PROPERTIES LINK_FLAGS "${ALL_LINKFLAGS}")
target_link_libraries(qsvn svnqt-qt4 ${QT_LIBRARIES})
