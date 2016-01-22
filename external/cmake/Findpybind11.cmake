#########################
# Finds pybind11
#########################

find_path(PYBIND11_INCLUDE_DIR NAMES pybind11/pybind11.h)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(pybind11 DEFAULT_MSG
                                  PYBIND11_INCLUDE_DIR)
