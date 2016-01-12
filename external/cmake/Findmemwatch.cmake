#########################
# Finds memwatch
#########################


find_path(MEMWATCH_INCLUDE_DIR NAMES memwatch/memwatch.h)

find_library(MEMWATCH_LIBRARIES NAMES memwatch)


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(memwatch DEFAULT_MSG
                                  MEMWATCH_INCLUDE_DIR
                                  MEMWATCH_LIBRARIES)
