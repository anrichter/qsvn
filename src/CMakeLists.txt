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
INCLUDE( ${CMAKE_SOURCE_DIR}/svnqt/cmakemodules/FindSubversion.cmake )

SET ( LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/lib )
SET ( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin )

INCLUDE_DIRECTORIES(
                     ${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR}/svnqt/buildqt4
                     ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}/svnqt/buildqt4
                     ${QT_INCLUDES}
                     ${APR_INCLUDE_DIR}
                     ${APU_INCLUDE_DIR}
                     ${SUBVERSION_INCLUDE_DIR}
)

ADD_SUBDIRECTORY( svnqt/buildqt4 )

SET( qsvn_headers
         addworkingcopy.h
         checkout.h
         config.h
         configure.h
         filelistproxy.h
         fileselector.h
         fileselectorproxy.h
         listener.h
         logchangepathentriesmodel.h
         logentriesmodel.h
         login.h
         qsvn.h
         showlog.h
         sslservertrust.h
         statusentriesmodel.h
         statustext.h
         svnclient.h
         version.h
         workingcopyitem.h
         workingcopymodel.h
)

SET( qsvn_sources
         addworkingcopy.cpp
         checkout.cpp
         config.cpp
         configure.cpp
         filelistproxy.cpp
         fileselector.cpp
         fileselectorproxy.cpp
         listener.cpp
         logchangepathentriesmodel.cpp
         logentriesmodel.cpp
         login.cpp
         main.cpp
         qsvn.cpp
         showlog.cpp
         sslservertrust.cpp
         statusentriesmodel.cpp
         statustext.cpp
         svnclient.cpp
         workingcopyitem.cpp
         workingcopymodel.cpp
)

SET( qsvn_forms
         forms/addworkingcopy.ui
         forms/checkout.ui
         forms/configure.ui
         forms/fileselector.ui
         forms/login.ui
         forms/qsvn.ui
         forms/showlog.ui
         forms/sslservertrust.ui
)

QT4_WRAP_UI( ui_headers ${qsvn_forms} )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )
QT4_AUTOMOC( ${qsvn_sources} )
QT4_ADD_RESOURCES( resfiles qsvn.qrc )

IF ( WIN32 )
    ADD_EXECUTABLE( qsvn WIN32 ${qsvn_sources} ${ui_headers} ${resfiles} qsvn.rc )
ELSE ( WIN32 )
    ADD_EXECUTABLE( qsvn ${qsvn_sources} ${ui_headers} ${resfiles} )
ENDIF( WIN32 )

SET_TARGET_PROPERTIES( qsvn PROPERTIES LINK_FLAGS "${ALL_LINKFLAGS}" )
TARGET_LINK_LIBRARIES( qsvn svnqt-qt4 ${QT_LIBRARIES} )

IF (WIN32)
    TARGET_LINK_LIBRARIES( qsvn ${QT_QTMAIN_LIBRARY} )
    TARGET_LINK_LIBRARIES( qsvn qtmain.lib ) #link qtmain.lib hard until cmake doesn't fill QT_QTMAIN_LIBRARY
ENDIF (WIN32)


IF ( UNIX )
    INSTALL( TARGETS qsvn DESTINATION bin )
ENDIF ( UNIX )
