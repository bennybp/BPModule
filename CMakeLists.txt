cmake_minimum_required(VERSION 3.2)
project(Pulsar CXX)
enable_language(C)


include(ExternalProject)


set(PULSAR_MATH_LIBS "" CACHE STRING "Linker flags/libraries for math libraries")
set(PULSAR_BUILD_TYPE "" CACHE STRING "Build type for Pulsar (as opposed to all the external dependencies)")
#set(PULSAR_PYTHON_VERSION "" CACHE STRING "Python version to use (example, 3.3)")

############################
# Build type
############################
if(NOT PULSAR_BUILD_TYPE)
    message(STATUS "PULSAR_BUILD_TYPE not specified. Using CMAKE_BUILD_TYPE (=${CMAKE_BUILD_TYPE})")
    set(PULSAR_BUILD_TYPE ${CMAKE_BUILD_TYPE})
endif()

############################
# Find some base libraries
############################
find_package(MPI REQUIRED)

#############
# MATH
#############
if(NOT PULSAR_MATH_LIBS)
  message(STATUS "PULSAR_MATH_LIBS not specified. Trying to find some")
  find_package(LAPACK REQUIRED)
  message(STATUS "Using LAPACK: ${LAPACK_LINKER_FLAGS} ${LAPACK_LIBRARIES}")
  message(STATUS "Using BLAS:   ${BLAS_LINKER_FLAGS} ${BLAS_LIBRARIES}")

  list(APPEND PULSAR_MATH_LIBS ${LAPACK_LINKER_FLAGS})
  list(APPEND PULSAR_MATH_LIBS ${LAPACK_LIBRARIES})
  list(APPEND PULSAR_MATH_LIBS ${BLAS_LINKER_FLAGS})
  list(APPEND PULSAR_MATH_LIBS ${BLAS_LIBRARIES})
endif()


message(STATUS "Using math libraries: ${PULSAR_MATH_LIBS}")


#############
# PYTHON
#############
set(Python_ADDITIONAL_VERSIONS "3.3;3.4;3.5;3.6")
find_package(PythonLibs "3.3" REQUIRED)
find_package(PythonInterp ${PYTHONLIBS_VERSION_STRING} EXACT REQUIRED)
message(STATUS "Python version: ${PYTHONLIBS_VERSION_STRING}")
message(STATUS "Python libs: ${PYTHON_LIBRARIES}")
message(STATUS "Python include: ${PYTHON_INCLUDE_DIRS}")
message(STATUS "Python executable: ${PYTHON_EXECUTABLE}")


#################################
# Add external projects
#################################
add_subdirectory(external)



#################################
# Core Pulsar
#################################
set(STAGE_DIR ${CMAKE_BINARY_DIR}/stage)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/cmake)
ExternalProject_Add(pulsar_core
   SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/pulsar
   CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
              -DCMAKE_BUILD_TYPE=${PULSAR_BUILD_TYPE}
              -DDESTDIR=${STAGE_DIR} # Pass in for searching for dependencies 
              -DCMAKE_PREFIX_PATH=${STAGE_DIR}/${CMAKE_INSTALL_PREFIX}/external
              -DMPI_CXX_COMPILER=${MPI_CXX_COMPILER}
              -DMPI_C_COMPILER=${MPI_C_COMPILER}
              -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
              -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
              -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
              -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
   CMAKE_CACHE_ARGS -DPULSAR_CORE_PYTHON_LIBRARIES:LIST=${PYTHON_LIBRARIES}
                    -DPULSAR_CORE_PYTHON_INCLUDE_DIRS:LIST=${PYTHON_INCLUDE_DIRS}
                    -DCMAKE_PREFIX_PATH:STRING=${CMAKE_PREFIX_PATH}
                    -DCMAKE_MODULE_PATH:STRING=${CMAKE_MODULE_PATH}
                    -DPULSAR_MATH_LIBS:LIST=${PULSAR_MATH_LIBS}
                    -DEIGEN3_INCLUDE_DIR:LIST=${EIGEN3_INCLUDE_DIR}
   BUILD_COMMAND $(MAKE)
   BUILD_ALWAYS 1
   INSTALL_COMMAND $(MAKE) install DESTDIR=${STAGE_DIR}
)
add_dependencies(pulsar_core memwatch_external
                             bphash_external
                             pybind11_external
                             libtaskforce_external
               )
ExternalProject_Add(pulsar_test
   SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test
   BINARY_DIR ${CMAKE_BINARY_DIR}/test   
   CMAKE_ARGS -DCMAKE_BUILD_TYPE=${PULSAR_BUILD_TYPE}
              -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
              -DMPI_CXX_COMPILER=${MPI_CXX_COMPILER}
              -DMPI_C_COMPILER=${MPI_C_COMPILER}
              -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
              -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
              -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
              -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
              -DPYTHON_EXECUTABLE=${PYTHON_EXECUTABLE}
   CMAKE_CACHE_ARGS -DCMAKE_PREFIX_PATH:STRING=${CMAKE_PREFIX_PATH};
                                            ${STAGE_DIR}/${CMAKE_INSTALL_PREFIX}
                    -DCMAKE_MODULE_PATH:STRING=${CMAKE_MODULE_PATH}
   BUILD_COMMAND $(MAKE)
   BUILD_ALWAYS 1
   INSTALL_COMMAND $(MAKE) install
)
add_dependencies(pulsar_test pulsar_core)
#################################
# Installation
#################################

# Install the staging directory
install(DIRECTORY ${CMAKE_BINARY_DIR}/stage/${CMAKE_INSTALL_PREFIX}/ 
        DESTINATION ${CMAKE_INSTALL_PREFIX} USE_SOURCE_PERMISSIONS)

# Install some random stuff
install(DIRECTORY basis DESTINATION ${CMAKE_INSTALL_PREFIX})
