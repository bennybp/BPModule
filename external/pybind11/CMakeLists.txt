cmake_minimum_required(VERSION 3.0)

###############################
# Detect and build pybind11
# if necessary
###############################

include(ExternalProject)

# 1.) Find if it already exists?
#     (it's not required yet)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
find_package(pybind11)


# 2.) If found, use those paths
if(${pybind11_FOUND})
  set(EXTERNAL_PYBIND11_INCLUDE_DIR "${pybind11_INCLUDE_DIR}")

# 3.) If not found, build it
else()
  ExternalProject_Add(pybind11_external_build
                      SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/pybind11-source
                      CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DPYBIND11_PYTHON_VERSION=${PYBIND11_PYTHON_VERSION}
                      INSTALL_COMMAND make install DESTDIR=${DESTDIR}
                     )

  # And use those paths.
  # Can't use find_package here because that gets run at configure time...
  set(EXTERNAL_PYBIND11_INCLUDE_DIR "include")  # Relative to install directory

endif()

# 3.) Create the import/export library
add_library(pybind11_interface INTERFACE)
target_include_directories(pybind11_interface INTERFACE
       $<INSTALL_INTERFACE:${EXTERNAL_PYBIND11_INCLUDE_DIR}>
)


# 4.) Install stuff and create the export file for use with bpmodule core project
install(TARGETS pybind11_interface EXPORT pybind11)

install(FILES   pybind11Config.cmake DESTINATION cmake)
install(EXPORT  pybind11 NAMESPACE External:: DESTINATION cmake)
