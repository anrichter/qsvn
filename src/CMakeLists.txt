ADD_SUBDIRECTORY( svnqt/buildqt4 )

SET( qsvn_headers
         addworkingcopy.h
         checkout.h
         config.h
         configure.h
         filelistitem.h
         filelistmodel.h
         filelistproxy.h
         fileselector.h
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
         filelistitem.cpp
         filelistmodel.cpp
         filelistproxy.cpp
         fileselector.cpp
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
         ${CMAKE_SOURCE_DIR}/src/forms/addworkingcopy.ui
         ${CMAKE_SOURCE_DIR}/src/forms/checkout.ui
         ${CMAKE_SOURCE_DIR}/src/forms/configure.ui
         ${CMAKE_SOURCE_DIR}/src/forms/fileselector.ui
         ${CMAKE_SOURCE_DIR}/src/forms/login.ui
         ${CMAKE_SOURCE_DIR}/src/forms/qsvn.ui
         ${CMAKE_SOURCE_DIR}/src/forms/showlog.ui
         ${CMAKE_SOURCE_DIR}/src/forms/sslservertrust.ui
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
