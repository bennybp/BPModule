find_package(PkgConfig)
pkg_search_module(PC_EIGEN3 QUIET eigen3)

find_path(FIND_EIGEN3_INCLUDE_DIR Eigen/Core
          HINTS ${PC_EIGEN3_INCLUDEDIR}
          PATHS ${EIGEN3_INCLUDE_DIR}
          PATH_SUFFIXES Eigen eigen3
)
message(STATUS ${FIND_EIGEN3_INCLUDE_DIR})
find_package_handle_standard_args(EIGEN3 DEFAULT_MSG FIND_EIGEN3_INCLUDE_DIR)
set(EIGEN3_INCLUDE_DIR ${FIND_EIGEN3_INCLUDE_DIR})
set(EIGEN3_FOUND TRUE)
