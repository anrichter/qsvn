PROJECT( qsvn )
CMAKE_MINIMUM_REQUIRED( VERSION 2.4.0 )

SET( QSVN_MAJOR_VERSION 0 )
SET( QSVN_MINOR_VERSION 6 )
SET( QSVN_MICRO_VERSION 0 )
SET( QSVN_BUILD_VERSION 0 )

SET( QSVN_VERSION_STRING "${QSVN_MAJOR_VERSION}.${QSVN_MINOR_VERSION}.${QSVN_MICRO_VERSION}")
SET( QSVN_VERSION_NUMBER ${QSVN_MAJOR_VERSION}.${QSVN_MINOR_VERSION}.${QSVN_MICRO_VERSION})

IF(EXISTS ${PROJECT_SOURCE_DIR}/.svn)
  FIND_PACKAGE(Subversion)
  IF(Subversion_FOUND)
    Subversion_WC_INFO(${PROJECT_SOURCE_DIR} QSvn)
    SET( QSVN_VERSION_STRING "${QSVN_VERSION_STRING} Build ${QSvn_WC_REVISION}" )
    SET( QSVN_VERSION_NUMBER ${QSVN_VERSION_NUMBER}.${QSvn_WC_REVISION} )
    SET( QSVN_BUILD_VERSION ${QSvn_WC_REVISION} )
  ENDIF(Subversion_FOUND)
ENDIF(EXISTS ${PROJECT_SOURCE_DIR}/.svn)

SET( LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib )

INCLUDE( ${CMAKE_ROOT}/Modules/FindQt4.cmake )
INCLUDE( ${CMAKE_ROOT}/Modules/UseQt4.cmake )
INCLUDE( ${CMAKE_SOURCE_DIR}/svnqt/cmakemodules/FindSubversion.cmake )

SET ( LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin )
SET ( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin )

INCLUDE_DIRECTORIES(
                     ${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
                     ${QT_INCLUDES}
                     ${APR_INCLUDE_DIR}
                     ${APU_INCLUDE_DIR}
                     ${SUBVERSION_INCLUDE_DIR}
)

ADD_SUBDIRECTORY( svnqt )

SET( qsvn_headers
         checkout.h
         config.h
         configure.h
         filelistproxy.h
         filemodifier.h
         fileselector.h
         fileselectorproxy.h
         listener.h
         logchangepathentriesmodel.h
         logentriesmodel.h
         login.h
         pathproperties.h
         pathpropertiesdelegate.h
         pathpropertiesmodel.h
         qsvn.h
         showlog.h
         sslservertrust.h
         statusentriesmodel.h
         statustext.h
         svnclient.h
         wcmodel.h
         ${CMAKE_CURRENT_BINARY_DIR}/qsvn_defines.h
)

SET( qsvn_sources
         checkout.cpp
         config.cpp
         configure.cpp
         filelistproxy.cpp
         filemodifier.cpp
         fileselector.cpp
         fileselectorproxy.cpp
         listener.cpp
         logchangepathentriesmodel.cpp
         logentriesmodel.cpp
         login.cpp
         main.cpp
         pathproperties.cpp
         pathpropertiesdelegate.cpp
         pathpropertiesmodel.cpp
         qsvn.cpp
         showlog.cpp
         sslservertrust.cpp
         statusentriesmodel.cpp
         statustext.cpp
         svnclient.cpp
         wcmodel.cpp
)

SET( qsvn_forms
         forms/checkout.ui
         forms/configure.ui
         forms/fileselector.ui
         forms/filemodifier.ui
         forms/login.ui
         forms/pathproperties.ui
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

IF (WIN32)
    SET(_SVNQT_EXPORT "__declspec(dllimport)")
    SET(TOUTF8 "toUtf8")
    SET(FROMUTF8 "fromUtf8")
    SET(QLIST "QList")
    CONFIGURE_FILE(
        "${CMAKE_CURRENT_SOURCE_DIR}/svnqt/svnqt_defines.hpp.in"
        "${CMAKE_CURRENT_BINARY_DIR}/svnqt-import/svnqt/svnqt_defines.hpp"
        IMMEDIATE
        @ONLY)
    INCLUDE_DIRECTORIES(BEFORE ${CMAKE_CURRENT_BINARY_DIR}/svnqt-import)
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
                            -dQSvnLibDir=${LIBRARY_OUTPUT_PATH}
                            -dQSvnBinDir=${EXECUTABLE_OUTPUT_PATH}
                            -dQSvnWiXDir=${CMAKE_SOURCE_DIR}/setup
                            -dQSvnMajorVersion=${QSVN_MAJOR_VERSION}
                            -dQSvnMinorVersion=${QSVN_MINOR_VERSION}
                            -dQSvnMicroVersion=${QSVN_MICRO_VERSION}
                            -dQSvnBuildVersion=${QSVN_BUILD_VERSION}
                            -dQtLibraryDir=${QT_LIBRARY_DIR}
                            -dSvnBinDir=${SUBVERSION_BIN_DIR}
                            ${CMAKE_SOURCE_DIR}/setup/qsvn.wxs
                        COMMAND light
                            -out setup/qsvn-${QSVN_MAJOR_VERSION}.${QSVN_MINOR_VERSION}.${QSVN_MICRO_VERSION}-${QSVN_BUILD_VERSION}.msi
                            qsvn.wixobj )
ENDIF ( WIN32 )

# Unit-Tests
ENABLE_TESTING()
ADD_SUBDIRECTORY( tests )
