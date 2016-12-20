cmake_minimum_required(VERSION 3.2)
project(Pulsar CXX)
enable_language(C)


include(ExternalProject)

#####################################################
# Some custom cmake Find* modules
# These are kept with the pulsar "core" project
# so that it can be used by that. But we can also
# use it here
#####################################################
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/pulsar/cmake")


set(PULSAR_MATH_LIBS "" CACHE STRING "Linker flags/libraries for math libraries")
set(PULSAR_BUILD_TYPE "" CACHE STRING "Build type for Pulsar (as opposed to all the external dependencies)")

############################
# Build type
############################
if(NOT PULSAR_BUILD_TYPE)
    message(STATUS "PULSAR_BUILD_TYPE not specified. Using CMAKE_BUILD_TYPE (=${CMAKE_BUILD_TYPE})")
    set(PULSAR_BUILD_TYPE ${CMAKE_BUILD_TYPE})
endif()


############################
# C++14 by default
############################
set(CMAKE_CXX_EXTENSIONS False CACHE BOOL "Enable/Disable compiler-specific C++ extensions")
set(CMAKE_CXX_STANDARD 14 CACHE STRING "Which C++ standard to use")


############################
# Find some MPI library
############################
find_package(MPI REQUIRED)
message(STATUS "MPI C Compiler: ${MPI_C_COMPILER}")
message(STATUS "MPI CXX Compiler: ${MPI_CXX_COMPILER}")
message(STATUS "MPI CXX Compile Flags: ${MPI_CXX_COMPILE_FLAGS}")
message(STATUS "MPI CXX Link Flags: ${MPI_CXX_LINK_FLAGS}")


#################
# Math libraries
#################
if(NOT PULSAR_MATH_LIBS)
  message(STATUS "PULSAR_MATH_LIBS not specified. Trying to find some")

  find_package(LAPACK REQUIRED)
  message(STATUS "LAPACK Linker Flags: ${LAPACK_LINKER_FLAGS}")
  message(STATUS "LAPACK Libraries: ${LAPACK_LIBRARIES}")
  message(STATUS "BLAS Linker Flags: ${BLAS_LINKER_FLAGS}")
  message(STATUS "BLAS Libraries: ${BLAS_LIBRARIES}")

  list(APPEND PULSAR_MATH_LIBS ${LAPACK_LINKER_FLAGS} ${LAPACK_LIBRARIES})
  list(APPEND PULSAR_MATH_LIBS ${BLAS_LINKER_FLAGS} ${BLAS_LIBRARIES})
endif()

message(STATUS "Using math libraries: ${PULSAR_MATH_LIBS}")


#####################
# Python libraries
#####################
set(Python_ADDITIONAL_VERSIONS "3.3;3.4;3.5;3.6;3.7")
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

##################################################
# Where stuff will be installed. This is passed
# To other external projects
##################################################
# Note that CMAKE_INSTALL_PREFIX is always absolute
# (cmake makes it that way). Therefore it always begins
# with a slash
set(STAGE_DIR         ${CMAKE_BINARY_DIR}/stage)
set(INSTALL_STAGE_DIR ${STAGE_DIR}${CMAKE_INSTALL_PREFIX}) 


#################################
# Core Pulsar
#################################
list(APPEND PULSAR_MODULE_PATH ${CMAKE_MODULE_PATH})
list(APPEND PULSAR_PREFIX_PATH ${INSTALL_STAGE_DIR} ${CMAKE_PREFIX_PATH})

message(STATUS "Pulsar prefix path: ${PULSAR_PREFIX_PATH}")

ExternalProject_Add(pulsar_core
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/pulsar
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
               -DCMAKE_BUILD_TYPE=${PULSAR_BUILD_TYPE}
               -DMPI_CXX_COMPILER=${MPI_CXX_COMPILER}
               -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
               -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
               -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
               -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
    CMAKE_CACHE_ARGS -DPULSAR_CORE_PYTHON_LIBRARIES:LIST=${PYTHON_LIBRARIES}
                     -DPULSAR_CORE_PYTHON_INCLUDE_DIRS:LIST=${PYTHON_INCLUDE_DIRS}
                     -DCMAKE_PREFIX_PATH:LIST=${PULSAR_PREFIX_PATH}
                     -DCMAKE_MODULE_PATH:LIST=${PULSAR_MODULE_PATH}
                     -DPULSAR_MATH_LIBS:LIST=${PULSAR_MATH_LIBS}
                     -DEIGEN3_INCLUDE_DIR:LIST=${EIGEN3_INCLUDE_DIR}
    BUILD_ALWAYS 1
    INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install DESTDIR=${STAGE_DIR}
)
add_dependencies(pulsar_core memwatch_external
                             bphash_external
                             bpprint_external
                             pybind11_external
                             libtaskforce_external
)

####################
# Pulsar core tests
####################
list(APPEND PULSAR_TEST_PREFIX_PATH ${STAGE_DIR}/${CMAKE_INSTALL_PREFIX})
list(APPEND PULSAR_TEST_PREFIX_PATH ${CMAKE_PREFIX_PATH})

ExternalProject_Add(pulsar_test
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/test_stage
               -DCMAKE_BUILD_TYPE=${PULSAR_BUILD_TYPE}
               -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
               -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
               -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
               -DCMAKE_C_FLAGS=${CMAKE_C_FLAGS}
               -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
               -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
               -DPYTHON_EXECUTABLE=${PYTHON_EXECUTABLE}
    CMAKE_CACHE_ARGS -DCMAKE_PREFIX_PATH:LIST=${PULSAR_TEST_PREFIX_PATH}
                     -DCMAKE_MODULE_PATH:LIST=${PULSAR_MODULE_PATH}
    BUILD_ALWAYS 1
)
add_dependencies(pulsar_test pulsar_core)


# This file will allow us to run ctest in the top-level build dir
# Basically it just defers to the actual top-level CTestTestfile.cmake in the
# build directory for this project
file(WRITE ${CMAKE_BINARY_DIR}/CTestTestfile.cmake "subdirs(test_stage)")



#################################
# Installation of the superbuild
#################################

# Installation is basically installing the staging directory
# Install the staging directory
install(DIRECTORY ${CMAKE_BINARY_DIR}/stage/${CMAKE_INSTALL_PREFIX}/ 
        DESTINATION ${CMAKE_INSTALL_PREFIX} USE_SOURCE_PERMISSIONS)

# Install other files, such as basis set
install(DIRECTORY basis DESTINATION ${CMAKE_INSTALL_PREFIX})

