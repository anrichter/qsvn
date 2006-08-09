#search libraries for Linux
IF (UNIX)

  FIND_PATH(SUBVERSION_INCLUDE_DIR svn_client.h
    /usr/include/subversion-1
    /usr/local/include/subversion-1
  )

  FIND_LIBRARY(SUBVERSION_CLIENTLIB svn_client-1
    /usr/lib
    /usr/local/lib
  )

  FIND_LIBRARY(SUBVERSION_REPOSITORYLIB svn_repos-1
    /usr/lib
    /usr/local/lib
  )

  FIND_PROGRAM(APR_CONFIG NAMES apr-config apr-1-config
    PATHS
    /usr/local/apr/bin
  )

  FIND_PROGRAM(APU_CONFIG NAMES apu-config apu-1-config
    PATHS
    /usr/local/apr/bin
  )

  if(NOT APR_CONFIG)
    MESSAGE(SEND_ERROR "Error: no apr-config found")
  endif(NOT APR_CONFIG)

  if(NOT APU_CONFIG)
    MESSAGE(SEND_ERROR "Error: no apu-config found")
  endif(NOT APU_CONFIG)

  EXEC_PROGRAM(${APR_CONFIG} ARGS "--includedir" OUTPUT_VARIABLE APR_INCLUDE_DIR)
  EXEC_PROGRAM(${APU_CONFIG} ARGS "--includedir" OUTPUT_VARIABLE APU_INCLUDE_DIR)
  EXEC_PROGRAM(${APR_CONFIG} ARGS "--ldflags --libs --link-ld" OUTPUT_VARIABLE APR_EXTRA_LIBFLAGS)
  EXEC_PROGRAM(${APU_CONFIG} ARGS "--ldflags --libs --link-ld" OUTPUT_VARIABLE APU_EXTRA_LIBFLAGS)

ENDIF (UNIX)

#search libaries for Windows
IF (WIN32)

  # search for pathes
  FIND_PATH (SUBVERSION_INCLUDE_DIR 
    svn_client.h
    "$ENV{ProgramFiles}/Subversion/include"
  )

  FIND_PATH(APR_INCLUDE_DIR apr.h
    "$ENV{ProgramFiles}/Subversion/include/apr"
  )

  # search for libraries
  FIND_LIBRARY(APR_LIB libapr
    "$ENV{ProgramFiles}/Subversion/lib/apr"
  )

  FIND_LIBRARY(APRICONV_LIB libapriconv
    "$ENV{ProgramFiles}/Subversion/lib/apr-iconv"
  )

  FIND_LIBRARY(APU_LIB libaprutil
    "$ENV{ProgramFiles}/Subversion/lib/apr-util"
  )

  FIND_LIBRARY(APU_XMLLIB xml
    "$ENV{ProgramFiles}/Subversion/lib/apr-util"
  )

  FIND_LIBRARY(NEON_LIB libneon
    "$ENV{ProgramFiles}/Subversion/lib/neon"
  )

  FIND_LIBRARY(NEON_ZLIBSTATLIB zlibstat
    "$ENV{ProgramFiles}/Subversion/lib/neon"
  )

  FIND_LIBRARY(DB43_LIB libdb43
    "$ENV{ProgramFiles}/Subversion/lib"
  )

  FIND_LIBRARY(SUBVERSION_CLIENTLIB libsvn_client-1
    "$ENV{ProgramFiles}/Subversion/lib"
  )

  FIND_LIBRARY(SUBVERSION_DELTALIB libsvn_delta-1
    "$ENV{ProgramFiles}/Subversion/lib"
  )

  FIND_LIBRARY(SUBVERSION_DIFFLIB libsvn_diff-1
    "$ENV{ProgramFiles}/Subversion/lib"
  )

  FIND_LIBRARY(SUBVERSION_FSBASELIB libsvn_fs_base-1
    "$ENV{ProgramFiles}/Subversion/lib"
  )

  FIND_LIBRARY(SUBVERSION_FSFSLIB libsvn_fs_fs-1
    "$ENV{ProgramFiles}/Subversion/lib"
  )

  FIND_LIBRARY(SUBVERSION_FSLIB libsvn_fs-1
    "$ENV{ProgramFiles}/Subversion/lib"
  )

  FIND_LIBRARY(SUBVERSION_RADAVLIB libsvn_ra_dav-1
    "$ENV{ProgramFiles}/Subversion/lib"
  )

  FIND_LIBRARY(SUBVERSION_RALOCALLIB libsvn_ra_local-1
    "$ENV{ProgramFiles}/Subversion/lib"
  )

  FIND_LIBRARY(SUBVERSION_RASVNLIB libsvn_ra_svn-1
    "$ENV{ProgramFiles}/Subversion/lib"
  )

  FIND_LIBRARY(SUBVERSION_RALIB libsvn_ra-1
    "$ENV{ProgramFiles}/Subversion/lib"
  )

  FIND_LIBRARY(SUBVERSION_REPOSITORYLIB libsvn_repos-1
    "$ENV{ProgramFiles}/Subversion/lib"
  )

  FIND_LIBRARY(SUBVERSION_SUBRLIB libsvn_subr-1
    "$ENV{ProgramFiles}/Subversion/lib"
  )

  FIND_LIBRARY(SUBVERSION_WCLIB libsvn_wc-1
    "$ENV{ProgramFiles}/Subversion/lib"
  )

  FIND_LIBRARY(SUBVERSION_INTL3LIB intl3_svn
    "$ENV{ProgramFiles}/Subversion/lib"
  )

  SET(APR_EXTRA_LIBFLAGS )
  SET(APU_EXTRA_LIBFLAGS )

ENDIF (WIN32)


IF(NOT SUBVERSION_INCLUDE_DIR)
  MESSAGE(SEND_ERROR "No subversion includes found!")
ENDIF(NOT SUBVERSION_INCLUDE_DIR)

if (NOT SUBVERSION_CLIENTLIB)
 MESSAGE(SEND_ERROR "No subversion client libs found!")
endif(NOT SUBVERSION_CLIENTLIB)

if (NOT SUBVERSION_REPOSITORYLIB)
 MESSAGE(SEND_ERROR "No subversion repository lib found!")
endif(NOT SUBVERSION_REPOSITORYLIB)
