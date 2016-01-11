#########################
# Finds libelemental
#########################


find_path(LIBELEMENTAL_INCLUDE_DIR NAMES El.hpp)

set(LIBELEMENTAL_INCLUDE_DIRS ${LIBELEMENTAL_INCLUDE_DIR})


find_library(LIBELEMENTAL_LIBRARIES
             PATHS ${LIBELEMENTAL_INCLUDE_DIRS}/../
             NAME El
             PATH_SUFFIXES lib) 


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(libelemental DEFAULT_MSG
                                  LIBELEMENTAL_INCLUDE_DIR
                                  LIBELEMENTAL_LIBRARIES)
