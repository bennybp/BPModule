#RMR- Stolen from https://github.com/justmoon/bzing/blob/master/cmake/FindBerkeleyDB.cmake
# -*- cmake -*-

# - Find BerkeleyDB
# Find the BerkeleyDB includes and library
# This module defines
#  DB_INCLUDE_DIR, where to find db.h, etc.
#  DB_LIBRARIES, the libraries needed to use BerkeleyDB.
#  DB_FOUND, If false, do not try to use BerkeleyDB.
# also defined, but not for general use are
#  DB_LIBRARY, where to find the BerkeleyDB library.
find_package(PkgConfig)
pkg_search_module(PC_DB QUIET db)

FIND_PATH(DB_INCLUDE_DIR db.h
  /usr/local/include/db4
  /usr/local/include
  /usr/include/db4
  /usr/include
  HINTS ${PC_DB_INCLUDE_DIRS}
  )

SET(DB_NAMES ${DB_NAMES} db db-5.3)
FIND_LIBRARY(DB_LIBRARY
  NAMES ${DB_NAMES}
  PATHS /usr/lib /usr/local/lib
  HINTS ${PC_DB_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(BerkeleyDB DEFAULT_MSG
                                     DB_INCLUDE_DIR DB_LIBRARY
)
