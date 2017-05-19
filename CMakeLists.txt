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
# Build type
############################
# Default to Release if build type not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

###################################################################
# Compilation flags
# Compile with many warnings enabled, but selectively
# disable useless ones
###################################################################
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w3")

    #  2338  : switch statement does not have a default clause
    #list(APPEND CMAKE_CXX_FLAGS "-ww2338")

    # some useless remarks
    #  1418  : external function definition with no prior declaration
    #  1419  : external declaration in primary source file 
    #   383  : value copied to temporary, reference to temporary used (generally ok in c++)
    #   981  : operands are evaluated in unspecified order (generally ok in c++)
    #   869  : parameter "XXX" was never referenced.
    #          This is due to a bug in Intel before Intel 16 that causes this
    #          remark to be printed for variadic templates
    # 11074  : Inlining inhibited by limit max-size (etc)
    # 11076  : To get full report use -qopt-report=4 -qopt-report-phase ipo
    #   444  : destructor for base class .. not virtual (even warns for classes without virtual functions)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -wd1418 -wd1419 -wd383 -wd981 -wd869")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -wd11074 -wd11076 -wd444")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfloat-equal -Wshadow")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wswitch-default -Wconversion")
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
macro(ADD_EXTRA_ARG extra_arg arg_name)
    if(${arg_name})
        set(${extra_arg} ${${extra_arg}}
                         -D${arg_name}=${${arg_name}})
    endif()
endmacro()

ADD_EXTRA_ARG(PULSAR_EXTRA_ARGS PYTHON_EXECUTABLE MPI_CXX_COMPILER)

ExternalProject_Add(pulsar
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/pulsar
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
               -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
               -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
               ${PULSAR_EXTRA_ARGS} 
    BUILD_ALWAYS 1
    INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} install DESTDIR=${STAGE_DIR}
    CMAKE_CACHE_ARGS -DCMAKE_PREFIX_PATH:LIST=${CMAKE_PREFIX_PATH}
                     -DCMAKE_INSTALL_RPATH:LIST=${CMAKE_INSTALL_RPATH}
                     -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
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
               -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
               ${PULSAR_EXTRA_ARGS}
    BUILD_ALWAYS 1
    CMAKE_CACHE_ARGS -DCMAKE_PREFIX_PATH:LIST=${PULSAR_PREFIX_PATH}
                     -DCMAKE_INSTALL_RPATH:LIST=${CMAKE_INSTALL_RPATH}
                     -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
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
install(DIRECTORY systems DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pulsar)

#####################
# uninstall target
#####################
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
