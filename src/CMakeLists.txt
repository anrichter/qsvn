SUBDIRS(svnqt/buildqt4)

FILE(GLOB uifiles forms/*.ui)
foreach(uifile ${uifiles})
 FILE(RELATIVE_PATH sfile ${QSVN_SOURCE_DIR}/src ${uifile})
 SET(inuifiles ${inuifiles} ${sfile})
endforeach(uifile)

QT4_WRAP_UI(ui_SOURCES ${inuifiles})

FILE(GLOB srces *.cpp)
FILE(GLOB headrs *.h)

MACRO(AUTOMOC filelist)
	SET(${filelist})
	FOREACH(_current ${ARGN})
		GET_FILENAME_COMPONENT(_abs_FILE ${_current} ABSOLUTE)
		IF (EXISTS ${_abs_FILE})
		  FILE(READ ${_abs_FILE} _contents)
		  STRING(REGEX MATCH "Q_OBJECT" _current_match "${_contents}")
	      if (_current_match)
			GET_filename_component(_basename ${_current} NAME_WE)
			SET(_mocout "${_basename}_moc.cxx")
			SET(${filelist} ${${filelist}} ${_mocout})
			QT4_GENERATE_MOC(${_current} ${_mocout})
	      endif (_current_match)

		ENDIF (EXISTS ${_abs_FILE})
	ENDFOREACH(_current)
ENDMACRO(AUTOMOC)

AUTOMOC(mocfiles ${headrs})
QT4_ADD_RESOURCES(resfiles qsvn.qrc)

ADD_EXECUTABLE(qsvn ${srces} ${ui_SOURCES} ${mocfiles} ${resfiles})

SET_SOURCE_FILES_PROPERTIES(${srces}
	PROPERTIES
	OBJECT_DEPENDS "${ui_SOURCES}")

SET_TARGET_PROPERTIES(qsvn PROPERTIES LINK_FLAGS "${ALL_LINKFLAGS}")
target_link_libraries(qsvn svnqt-qt4 ${QT_LIBRARIES})

