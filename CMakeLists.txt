cmake_minimum_required(VERSION 3.0)
project(BPModule CXX)
enable_language(C)


include(ExternalProject)


set(BPMODULE_MATH_LIBS "" CACHE STRING "Linker flags/libraries for math libraries")
#set(BPMODULE_PYTHON_VERSION "" CACHE STRING "Python version to use (example, 3.3)")


############################
# Find some base libraries
############################
find_package(MPI REQUIRED)

#############
# MATH
#############
if(NOT BPMODULE_MATH_LIBS)
  message(STATUS "BPMODULE_MATH_LIBS not specified. Trying to find some")
  find_package(LAPACK REQUIRED)
  message(STATUS "Using LAPACK: ${LAPACK_LINKER_FLAGS} ${LAPACK_LIBRARIES}")
  message(STATUS "Using BLAS:   ${BLAS_LINKER_FLAGS} ${BLAS_LIBRARIES}")

  list(APPEND BPMODULE_MATH_LIBS ${LAPACK_LINKER_FLAGS})
  list(APPEND BPMODULE_MATH_LIBS ${LAPACK_LIBRARIES})
  list(APPEND BPMODULE_MATH_LIBS ${BLAS_LINKER_FLAGS})
  list(APPEND BPMODULE_MATH_LIBS ${BLAS_LIBRARIES})
endif()


message(STATUS "Using math libraries: ${BPMODULE_MATH_LIBS}")


#############
# PYTHON
#############
set(Python_ADDITIONAL_VERSIONS "3.3;3.4;3.5;3.6")
find_package(PythonLibs "3.3" REQUIRED)
message(STATUS "Python libs: ${PYTHON_LIBRARIES}")
message(STATUS "Python include: ${PYTHON_INCLUDE_DIRS}")


#################################
# Add external projects
#################################
add_subdirectory(external)



#################################
# Core BPModule
#################################
ExternalProject_Add(bpmodule_core
                    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bpmodule
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                               -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                               -DDESTDIR=${CMAKE_BINARY_DIR}/stage                   # Pass in to allow for searching for dependencies 
                               -DCMAKE_PREFIX_PATH=${CMAKE_BINARY_DIR}/stage/${CMAKE_INSTALL_PREFIX}/external
                               -DMPI_CXX_COMPILER=${MPI_CXX_COMPILER}
                               -DMPI_C_COMPILER=${MPI_C_COMPILER}
                    CMAKE_CACHE_ARGS -DBPMODULE_CORE_PYTHON_LIBRARIES:STRING=${PYTHON_LIBRARIES}
                                     -DBPMODULE_CORE_PYTHON_INCLUDE_DIRS:STRING=${PYTHON_INCLUDE_DIRS}
                                     -DCMAKE_PREFIX_PATH:STRING=${CMAKE_PREFIX_PATH}
                    INSTALL_COMMAND make install DESTDIR=${CMAKE_BINARY_DIR}/stage
)
add_dependencies(bpmodule_core memwatch_external
                               pybind11_external
                               libelemental_external
                               madness_external
                )


#################################
# Installation
#################################

# Install the staging directory
install(DIRECTORY ${CMAKE_BINARY_DIR}/stage/${CMAKE_INSTALL_PREFIX}/ DESTINATION ${CMAKE_INSTALL_PREFIX} USE_SOURCE_PERMISSIONS)

# Install some random stuff
install(DIRECTORY basis DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY test DESTINATION ${CMAKE_INSTALL_PREFIX} USE_SOURCE_PERMISSIONS)
