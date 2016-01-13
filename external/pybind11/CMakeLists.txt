###############################
# Detect and build pybind11
# if necessary
###############################

include(ExternalProject)

# Find if it already exists? (it's not required yet)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
find_package(pybind11)


# If not found, build it
if(NOT pybind11_FOUND)
  message(STATUS "We will build pybind11 for you")
  ExternalProject_Add(pybind11_external
                      SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/pybind11-source
                      CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/external/pybind11
                                 -DPYBIND11_PYTHON_VERSION=${PYBIND11_PYTHON_VERSION}
                                 -DPYBIND11_INSTALL_ONLY:Bool=On
                                 -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                                 -DCMAKE_CXX_FLAGS="-std=c++11"
                      INSTALL_COMMAND make install DESTDIR=${CMAKE_BINARY_DIR}/stage
                     )
else()
  message(STATUS "Using existing pybind11")

  # Add a dummy library for dependencies
  add_library(pybind11_external INTERFACE)
endif()
