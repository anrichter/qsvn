PROJECT(qsvn)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4.0)
#Set Policy for cmake version 2.6.0 and above
IF(COMMAND cmake_policy)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND cmake_policy)

INCLUDE (CheckCXXCompilerFlag)

SET(QSVN_MAJOR_VERSION 0)
SET(QSVN_MINOR_VERSION 8)
SET(QSVN_MICRO_VERSION 90)
SET(QSVN_BUILD_VERSION 0)

SET(QSVN_VERSION_STRING "${QSVN_MAJOR_VERSION}.${QSVN_MINOR_VERSION}.${QSVN_MICRO_VERSION}")
SET(QSVN_VERSION_NUMBER ${QSVN_MAJOR_VERSION}.${QSVN_MINOR_VERSION}.${QSVN_MICRO_VERSION})

CHECK_CXX_COMPILER_FLAG(-fvisibility=hidden HAVE_GCC_VISIBILITY)
SET(HAVE_GCC_VISIBILITY ${HAVE_GCC_VISIBILITY} CACHE BOOL "GCC support for hidden visibility")

IF (EXISTS ${PROJECT_SOURCE_DIR}/.svn)
  FIND_PACKAGE(Subversion)
  IF (Subversion_FOUND)
    SUBVERSION_WC_INFO(${PROJECT_SOURCE_DIR} QSvn)
    SET(QSVN_VERSION_STRING "${QSVN_VERSION_STRING} Build ${QSvn_WC_REVISION}")
    SET(QSVN_VERSION_NUMBER ${QSVN_VERSION_NUMBER}.${QSvn_WC_REVISION})
    SET(QSVN_BUILD_VERSION ${QSvn_WC_REVISION})
  ENDIF (Subversion_FOUND)
ENDIF (EXISTS ${PROJECT_SOURCE_DIR}/.svn)

FIND_PACKAGE(Qt4)
SET(QT_USE_QTMAIN 1)
INCLUDE(${QT_USE_FILE})

INCLUDE(${CMAKE_SOURCE_DIR}/svnqt/cmakemodules/FindSubversion.cmake)

IF (WIN32)
  SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
  SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
ELSE (WIN32)
  IF(CMAKE_SIZEOF_VOID_P EQUAL 4)
    SET(LIB_SUFFIX "")
  ELSE(CMAKE_SIZEOF_VOID_P EQUAL 4)
    SET(LIB_SUFFIX 64)
  ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 4)
  SET(LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX})
  SET(BIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/bin)
  SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib${LIB_SUFFIX})
  SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
ENDIF (WIN32)

INCLUDE_DIRECTORIES(
                     ${CMAKE_CURRENT_BINARY_DIR}
                     ${CMAKE_CURRENT_SOURCE_DIR}
                     ${QT_INCLUDES}
                     ${APR_INCLUDE_DIR}
                     ${APU_INCLUDE_DIR}
                     ${SUBVERSION_INCLUDE_DIR}
)

ADD_SUBDIRECTORY(svnqt)

SET(qsvn_headers
         checkout.h
         config.h
         configure.h
         filelistproxy.h
         filemodifier.h
         fileselector.h
         fileselectorproxy.h
         helper.h
         listener.h
         logchangepathentriesmodel.h
         logentriesmodel.h
         login.h
         merge.h
         pathproperties.h
         pathpropertiesmodel.h
         propertyedit.h
         qarfilesystemwatcher.h
         qsvn.h
         showlog.h
         sslservertrust.h
         statusentriesmodel.h
         statustext.h
         svnclient.h
         textedit.h
         wcmodel.h
         ${CMAKE_CURRENT_BINARY_DIR}/qsvn_defines.h
         qsvnactions/qsvnaction.h
         qsvnactions/qsvnrepositorycreateaction.h
         qsvnactions/qsvnrepositorylistener.h
)

SET(qsvn_sources
         checkout.cpp
         config.cpp
         configure.cpp
         filelistproxy.cpp
         filemodifier.cpp
         fileselector.cpp
         fileselectorproxy.cpp
         helper.cpp
         listener.cpp
         logchangepathentriesmodel.cpp
         logentriesmodel.cpp
         login.cpp
         main.cpp
         merge.cpp
         pathproperties.cpp
         pathpropertiesmodel.cpp
         propertyedit.cpp
         qarfilesystemwatcher.cpp
         qsvn.cpp
         showlog.cpp
         sslservertrust.cpp
         statusentriesmodel.cpp
         statustext.cpp
         svnclient.cpp
         textedit.cpp
         wcmodel.cpp
         qsvnactions/qsvnaction.cpp
         qsvnactions/qsvnrepositorycreateaction.cpp
         qsvnactions/qsvnrepositorylistener.h
)

SET(qsvn_forms
         forms/checkout.ui
         forms/configure.ui
         forms/fileselector.ui
         forms/filemodifier.ui
         forms/login.ui
         forms/merge.ui
         forms/pathproperties.ui
         forms/propertyedit.ui
         forms/qsvn.ui
         forms/showlog.ui
         forms/sslservertrust.ui
         forms/textedit.ui
)

QT4_WRAP_UI(ui_headers ${qsvn_forms})
QT4_AUTOMOC(${qsvn_sources})
QT4_ADD_RESOURCES(resfiles qsvn.qrc)

CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/qsvn_defines.h.in"
               "${CMAKE_CURRENT_BINARY_DIR}/qsvn_defines.h"
               IMMEDIATE
               @ONLY)

IF (WIN32)
    SET(_SVNQT_EXPORT "__declspec(dllimport)")
    SET(TOUTF8 "toUtf8")
    SET(FROMUTF8 "fromUtf8")
    SET(QLIST "QList")
    SET(TOASCII "toAscii")
    SET(HOMEDIR "homePath")
    SET(QDATABASE "QSqlDatabase")
    SET(QLONG "qlonglong")
    CONFIGURE_FILE(
        "${CMAKE_CURRENT_SOURCE_DIR}/svnqt/svnqt_defines.hpp.in"
        "${CMAKE_CURRENT_BINARY_DIR}/svnqt-import/svnqt/svnqt_defines.hpp"
        IMMEDIATE
        @ONLY)
    INCLUDE_DIRECTORIES(BEFORE ${CMAKE_CURRENT_BINARY_DIR}/svnqt-import)
    ADD_EXECUTABLE(qsvn WIN32 ${qsvn_sources} ${ui_headers} ${resfiles} qsvn.rc)
ELSE (WIN32)
    ADD_EXECUTABLE(qsvn ${qsvn_sources} ${ui_headers} ${resfiles})
ENDIF (WIN32)

SET_TARGET_PROPERTIES(qsvn PROPERTIES LINK_FLAGS "${ALL_LINKFLAGS}")
TARGET_LINK_LIBRARIES(qsvn svnqt ${QT_LIBRARIES})

IF (UNIX)
    INSTALL(TARGETS qsvn
      RUNTIME DESTINATION ${BIN_INSTALL_DIR}
      LIBRARY DESTINATION ${LIB_INSTALL_DIR})
ENDIF (UNIX)

IF (WIN32)
    ADD_CUSTOM_TARGET (setup
                        COMMAND ISCC.exe
                            ${CMAKE_SOURCE_DIR}/setup/qsvn.iss
                            /dQSvnSourceDir=${CMAKE_SOURCE_DIR}
                            /dQSvnBinDir=${EXECUTABLE_OUTPUT_PATH}
                            /dQSvnVersionString=${QSVN_VERSION_STRING}
                            /dQSvnVersionNumber=${QSVN_VERSION_NUMBER}
                            /dQtLibraryDir=${QT_LIBRARY_DIR}
                            /dQtPluginsDir=${QT_PLUGINS_DIR}
                            /dSvnBinDir=${SUBVERSION_BIN_DIR}
                            /dVcInstallDir=$ENV{VCINSTALLDIR}
                       )
ENDIF (WIN32)

# Unit-Tests
ENABLE_TESTING()
ADD_SUBDIRECTORY(tests)
ADD_SUBDIRECTORY(qsvnactions/tests)
