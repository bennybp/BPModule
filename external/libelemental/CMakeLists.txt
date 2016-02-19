include(ExternalProject)

# Find if it already exists? (it's not required yet)
find_package(libelemental QUIET)


# If not found, build it
if(NOT LIBELEMENTAL_FOUND)
  message(STATUS "We will build libelemental for you")
  ExternalProject_Add(libelemental_external
                      SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libelemental-source
                      CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/external/libelemental 
                                 #-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                                 -DBUILD_KISSFFT:Bool=Off
                                 -DBUILD_METIS:Bool=Off
                                 -DINSTALL_PYTHON_PACKAGE:Bool=No
                                 -DBUILD_SHARED_LIBS:Bool=No
                                 -DCMAKE_CXX_FLAGS=-fPIC
                                 -DMPI_CXX_COMPILER=${MPI_CXX_COMPILER}
                                 -DMPI_C_COMPILER=${MPI_C_COMPILER}
                                 #-DMPI_Fortran_COMPILER=${MPI_Fortran_COMPILER}
                                 -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                                 -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                                 -DCMAKE_Fortran_COMPILER=${CMAKE_Fortran_COMPILER}
                      CMAKE_CACHE_ARGS -DMATH_LIBS:STRING=${BPMODULE_MATH_LIBS}
                      INSTALL_COMMAND make install DESTDIR=${CMAKE_BINARY_DIR}/stage
                     )
else()
  # Add a dummy library for dependencies
  add_library(libelemental_external INTERFACE)
endif()
