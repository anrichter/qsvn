FIND_PACKAGE(Qt4)
SET(QT_USE_QTTEST 1)
INCLUDE(${QT_USE_FILE})

SET ( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR}
                     ${QT_INCLUDES} )

set ( qsvn_sources
        ../qsvnaction.cpp
        ../qsvnrepositorycreateaction.cpp
        ../qsvnrepositorylistener.cpp
        ../../listener.cpp
        ../../login.cpp
        ../../statustext.cpp
        ../../sslservertrust.cpp )

set ( testcases
        qsvnrepositorycreateactiontests  )

foreach( testcase ${testcases} )
  QT4_GENERATE_MOC( ${CMAKE_CURRENT_SOURCE_DIR}/${testcase}.cpp ${CMAKE_CURRENT_BINARY_DIR}/${testcase}.moc )
  ADD_EXECUTABLE( ${testcase} ${testcase}.cpp ${qsvn_sources} ${CMAKE_CURRENT_BINARY_DIR}/${testcase}.moc)
  ADD_TEST( ${testcase} ${EXECUTABLE_OUTPUT_PATH}/${testcase} )
  TARGET_LINK_LIBRARIES( ${testcase} svnqt ${QT_LIBRARIES} )
endforeach( testcase ${testcases} )
