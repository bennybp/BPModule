include(ExternalProject)

# Find if it already exists? (it's not required yet)
find_package(pybind11 QUIET) 

# If not found, build it
if(NOT pybind11_FOUND)
  message(STATUS "We will build pybind11 for you")
  ExternalProject_Add(pybind11_external
                      GIT_REPOSITORY https://github.com/pybind/pybind11.git
                      GIT_TAG c79e435e00fd4fb49817a87c3c2b2647482c077b
                      CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                                 -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                                 -DPYTHON_LIBRARY=${PYTHON_LIBRARY}
                                 -DPYTHON_INCLUDE_DIR=${PYTHON_INCLUDE_DIR}
                                 -DPYTHON_EXECUTABLE=${PYTHON_EXECUTABLE}
                                 -DPYBIND11_PYTHON_VERSION=${PYTHONLIBS_VERSION_STRING}
                                 -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                                 -DPYBIND11_INSTALL=True
                                 -DPYBIND11_TEST=False
                      INSTALL_COMMAND make install DESTDIR=${CMAKE_BINARY_DIR}/stage
                     )
else()
  # Add a dummy library for dependencies
  add_library(pybind11_external INTERFACE)
endif()
