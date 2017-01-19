# Find BerkeleyDB
# Find the BerkeleyDB includes and library
#
# This module defines
#  BerkeleyDB_INCLUDE_DIRS, where to find db.h, etc.
#  BerkeleyDB_LIBRARIES, the libraries needed to use BerkeleyDB.
#  BerkeleyDB_FOUND, If false, do not try to use BerkeleyDB.

find_path(BerkeleyDB_INCLUDE_DIR db.h)

set(BerkeleyDB_NAMES ${BerkeleyDB_NAMES} db)
find_library(BerkeleyDB_LIBRARY
             NAMES ${BerkeleyDB_NAMES}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(BerkeleyDB DEFAULT_MSG
                                  BerkeleyDB_INCLUDE_DIR BerkeleyDB_LIBRARY
)

mark_as_advanced(BerkeleyDB_LIBRARY BerkeleyDB_INCLUDE_DIR)
set(BerkeleyDB_LIBRARIES ${BerkeleyDB_LIBRARY})
set(BerkeleyDB_INCLUDE_DIRS ${BerkeleyDB_INCLUDE_DIR})

