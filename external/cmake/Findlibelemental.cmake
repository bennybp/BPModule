#########################
# Finds libelemental
#########################


find_path(LIBELEMENTAL_INCLUDE_DIR NAMES El.hpp)

find_library(LIBELEMENTAL_LIBRARIES NAMES El pmrrr)


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(libelemental DEFAULT_MSG
                                  LIBELEMENTAL_INCLUDE_DIR
                                  LIBELEMENTAL_LIBRARIES)
