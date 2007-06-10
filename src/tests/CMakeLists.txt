INCLUDE( ${CMAKE_ROOT}/Modules/FindQt4.cmake )
INCLUDE( ${CMAKE_ROOT}/Modules/UseQt4.cmake )

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
  TARGET_LINK_LIBRARIES( ${testcase} ${QT_LIBRARIES} ${QT_QTTEST_LIBRARY} )
endforeach( testcase ${testcases} )
