FILE(GLOB SOURCES ../*.cpp)
FILE(GLOB HEADERS ../*.hpp)
ADD_LIBRARY(svnqt-qt4 SHARED ${SOURCES} )
SET_TARGET_PROPERTIES(svnqt-qt4 PROPERTIES SOVERSION 2 VERSION 2.2.1)
SET(svnqt-qt4_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
SET(ALL_LINKFLAGS ${APR_EXTRA_LIBFLAGS})
SET(ALL_LINKFLAGS "${ALL_LINKFLAGS} ${APU_EXTRA_LIBFLAGS}")

SET_TARGET_PROPERTIES(svnqt-qt4 PROPERTIES LINK_FLAGS "${ALL_LINKFLAGS}")
target_link_libraries(svnqt-qt4 ${QT_QTCORE_LIBRARY} ${SUBVERSION_CLIENTLIB} ${SUBVERSION_REPOSITORYLIB})

# Just a small linking test
ADD_EXECUTABLE(testlink testmain.cpp)
target_link_libraries(testlink svnqt-qt4)

# install rules
install_targets(/lib svnqt-qt4 svnqt-qt4)
install_files(/include/svnqt FILES ${HEADERS})
