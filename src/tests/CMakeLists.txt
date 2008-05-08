FIND_PACKAGE(Qt4)
SET(QT_USE_QTTEST 1)
INCLUDE(${QT_USE_FILE})

SET ( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_SOURCE_DIR}
                     ${CMAKE_CURRENT_BINARY_DIR}
                     ${QT_INCLUDES} )

set ( testcases
        simpletest
        simpletest2 )

foreach( testcase ${testcases} )
  QT4_GENERATE_MOC( ${CMAKE_CURRENT_SOURCE_DIR}/${testcase}.cpp ${CMAKE_CURRENT_BINARY_DIR}/${testcase}.moc )
  ADD_EXECUTABLE( ${testcase} ${testcase}.cpp )
  ADD_TEST( ${testcase} ${EXECUTABLE_OUTPUT_PATH}/${testcase} )
  TARGET_LINK_LIBRARIES( ${testcase} ${QT_LIBRARIES} )
endforeach( testcase ${testcases} )
