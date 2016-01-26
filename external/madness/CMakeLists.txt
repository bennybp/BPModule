include(ExternalProject)

# Find if it already exists? (it's not required yet)
find_package(MADNESS QUIET COMPONENTS world)


# If not found, build it
if(NOT MADNESS_FOUND)
  message(STATUS "We will build MADNESS for you")
  ExternalProject_Add(madness_external
                      SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/madness-source
                      CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/external/madness
                                 -DPYTHON_LIBRARY=${PYTHON_LIBRARY}
                                 -DPYTHON_INCLUDE_DIR=${PYTHON_INCLUDE_DIR}
                                 -DPYTHON_EXECUTABLE=${PYTHON_EXECUTABLE}
                                 -DENABLE_LIBXC=FALSE
                                 -DENABLE_GENTENSOR=FALSE
                                 -DENABLE_GPERFTOOLS=FALSE
                                 -DENABLE_UNITTESTS=FALSE
                                 -DCMAKE_CXX_FLAGS=-fPIC
                                 -DMPI_CXX_COMPILER=${MPI_CXX_COMPILER}
                                 -DMPI_C_COMPILER=${MPI_C_COMPILER}
                                 -DMPI_Fortran_COMPILER=${MPI_Fortran_COMPILER}
                                 -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                                 -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                                 -DCMAKE_Fortran_COMPILER=${CMAKE_Fortran_COMPILER}
                      CMAKE_CACHE_ARGS -DLAPACK_LIBRARIES:STRING=${BPMODULE_MATH_LIBS}
                      BUILD_COMMAND ${CMAKE_COMMAND} --build . --target MADworld
                      INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install-common install-world install-config DESTDIR=${CMAKE_BINARY_DIR}/stage
                     )
else()
  # Add a dummy library for dependencies
  add_library(madness_external INTERFACE)
endif()
