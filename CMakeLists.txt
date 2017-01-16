cmake_minimum_required(VERSION 3.2)
project(pulsar-superbuild CXX)

include(ExternalProject)

#####################################################
# Some custom cmake Find* modules
# These are kept with the pulsar "core" project
# so that it can be used by that. But we can also
# use it here
#####################################################
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/pulsar/cmake")


############################
# C++14 by default
############################
set(CMAKE_CXX_EXTENSIONS False CACHE BOOL "Enable/Disable compiler-specific C++ extensions")
set(CMAKE_CXX_STANDARD 14 CACHE STRING "Which C++ standard to use")


############################
# Build type
############################
# Default to Release if build type not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()


##################################################
# Where stuff will be installed. This is passed
# to other external projects
##################################################
# Note that CMAKE_INSTALL_PREFIX is always absolute
# (cmake makes it that way). Therefore it always begins
# with a slash
set(STAGE_DIR            "${CMAKE_BINARY_DIR}/stage")
set(STAGE_INSTALL_PREFIX "${STAGE_DIR}${CMAKE_INSTALL_PREFIX}")

#################################
# Core Pulsar
#################################
# Some arguments to pass to the core project ExternalProject_Add
# We do it this way since some find_package may have problems if
# a variable is set, but only to a blank string
# For example, find_package(PythonInterp) will fail
# if PYTHON_EXECUTABLE is a blank string
if(PYTHON_EXECUTABLE)
    set(PULSAR_EXTRA_ARGS ${PULSAR_EXTRA_ARGS}
                          -DPYTHON_EXECUTABLE=${PYTHON_EXECUTABLE})
endif()

ExternalProject_Add(pulsar
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/pulsar
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
               -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
               -DMPI_CXX_COMPILER=${MPI_CXX_COMPILER}
               -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
               -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
               -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
               -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
               ${PULSAR_EXTRA_ARGS} 
    BUILD_ALWAYS 1
    INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install DESTDIR=${STAGE_DIR}
    CMAKE_CACHE_ARGS -DCMAKE_PREFIX_PATH:LIST=${CMAKE_PREFIX_PATH}
                     -DCMAKE_INSTALL_RPATH:LIST=${CMAKE_INSTALL_RPATH}
)

####################
# Pulsar core tests
####################
# Where the core testing project should search for dependencies
# Ie, the original CMAKE_PREFIX_PATH, plus the installation stage
list(APPEND PULSAR_PREFIX_PATH ${STAGE_INSTALL_PREFIX} ${CMAKE_PREFIX_PATH})

ExternalProject_Add(pulsar_test
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/test_stage
               -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
               -DMPI_CXX_COMPILER=${MPI_CXX_COMPILER}
               -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
               -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
               -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
               -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
    BUILD_ALWAYS 1
    CMAKE_CACHE_ARGS -DCMAKE_PREFIX_PATH:LIST=${PULSAR_PREFIX_PATH}
                     -DCMAKE_INSTALL_RPATH:LIST=${CMAKE_INSTALL_RPATH}
)
add_dependencies(pulsar_test pulsar)


# This file will allow us to run ctest in the top-level build dir
# Basically it just defers to the actual top-level CTestTestfile.cmake in the
# build directory for this project
file(WRITE ${CMAKE_BINARY_DIR}/CTestTestfile.cmake "subdirs(test_stage)")


#################################
# Installation of the superbuild
#################################
# Installation is basically copying the staging directory
install(DIRECTORY ${CMAKE_BINARY_DIR}/stage/${CMAKE_INSTALL_PREFIX}/ 
        DESTINATION ${CMAKE_INSTALL_PREFIX} USE_SOURCE_PERMISSIONS)

# Install other files, such as basis set
install(DIRECTORY basis DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pulsar)
