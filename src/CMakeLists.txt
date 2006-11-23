PROJECT( qsvn )
SET( QSVN_MAJOR_VERSION 0 )
SET( QSVN_MINOR_VERSION 4 )
SET( QSVN_MICRO_VERSION 94 )

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
         workingcopyitem.h
         workingcopymodel.h
         ${CMAKE_CURRENT_BINARY_DIR}/qsvn_defines.h         
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

CONFIGURE_FILE( "${CMAKE_CURRENT_SOURCE_DIR}/qsvn_defines.h.in"
                "${CMAKE_CURRENT_BINARY_DIR}/qsvn_defines.h"
                IMMEDIATE
                @ONLY )

IF ( WIN32 )
    ADD_EXECUTABLE( qsvn WIN32 ${qsvn_sources} ${ui_headers} ${resfiles} qsvn.rc )
ELSE ( WIN32 )
    ADD_EXECUTABLE( qsvn ${qsvn_sources} ${ui_headers} ${resfiles} )
ENDIF( WIN32 )

SET_TARGET_PROPERTIES( qsvn PROPERTIES LINK_FLAGS "${ALL_LINKFLAGS}" )
TARGET_LINK_LIBRARIES( qsvn svnqt-qt4 ${QT_LIBRARIES} )

IF ( WIN32 )
    TARGET_LINK_LIBRARIES( qsvn ${QT_QTMAIN_LIBRARY} )
    TARGET_LINK_LIBRARIES( qsvn qtmain.lib ) #link qtmain.lib hard until cmake doesn't fill QT_QTMAIN_LIBRARY
ENDIF ( WIN32 )


IF ( UNIX )
    INSTALL( TARGETS qsvn DESTINATION bin )
ENDIF ( UNIX )

IF ( WIN32 )
    ADD_CUSTOM_TARGET ( wix
                        COMMAND candle 
                            -dQSvnSourceDir=${CMAKE_SOURCE_DIR}
                            -dQSvnBinDir=${EXECUTABLE_OUTPUT_PATH}
                            -dQSvnWiXDir=${CMAKE_SOURCE_DIR}/setup
                            -dQSvnMajorVersion=${QSVN_MAJOR_VERSION}
                            -dQSvnMinorVersion=${QSVN_MINOR_VERSION}
                            -dQSvnMicroVersion=${QSVN_MICRO_VERSION}
                            ${CMAKE_SOURCE_DIR}/setup/qsvn.wxs
                        COMMAND light 
                            -out setup/qsvn-${QSVN_MAJOR_VERSION}.${QSVN_MINOR_VERSION}.${QSVN_MICRO_VERSION}.msi 
                            qsvn.wixobj )
ENDIF ( WIN32 )
