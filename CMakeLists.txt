PROJECT( qsvn )

CMAKE_MINIMUM_REQUIRED( VERSION 2.4.0 )

IF ( UNIX )
  SET( QT_QMAKE_EXECUTABLE qmake-qt4 )
  SET( QT_MOC_EXECUTABLE moc-qt4 )
  SET( QT_UIC_EXECUTABLE uic-qt4 )
  SET( CMAKE_INSTALL_PREFIX /usr )
ENDIF ( UNIX )

INCLUDE( ${CMAKE_ROOT}/Modules/FindQt4.cmake )
INCLUDE( ${CMAKE_ROOT}/Modules/UseQt4.cmake )
INCLUDE( ${CMAKE_SOURCE_DIR}/src/svnqt/cmakemodules/FindSubversion.cmake )

SET ( LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/src/lib )
SET ( EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/src/bin )

INCLUDE_DIRECTORIES( ${QT_INCLUDES} 
                     ${APR_INCLUDE_DIR}
                     ${APU_INCLUDE_DIR}
                     ${SUBVERSION_INCLUDE_DIR}
                     ${QSVN_SOURCE_DIR}/src/svnqt 
)

ADD_SUBDIRECTORY( src )
