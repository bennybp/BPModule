cmake_minimum_required(VERSION 3.0)


##########################
# Options to be passed in
##########################
set(PULSAR_CORE_PYTHON_LIBRARIES "" CACHE STRING "Python libraries to link to")
set(PULSAR_CORE_PYTHON_INCLUDE_DIRS "" CACHE STRING "Include directories for python")


##########################
# Dependencies
##########################
find_package(OpenMP REQUIRED)
find_package(MPI REQUIRED)
find_package(Eigen3 REQUIRED)


##########################
# Dependencies in external
##########################
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../external/cmake")
list(APPEND CMAKE_PREFIX_PATH "${DESTDIR}/${CMAKE_INSTALL_PREFIX}/external/pybind11")
list(APPEND CMAKE_PREFIX_PATH "${DESTDIR}/${CMAKE_INSTALL_PREFIX}/external/memwatch")
list(APPEND CMAKE_PREFIX_PATH "${DESTDIR}/${CMAKE_INSTALL_PREFIX}/external/bphash")
list(APPEND CMAKE_PREFIX_PATH "${DESTDIR}/${CMAKE_INSTALL_PREFIX}/external/libtaskforce")
find_package(pybind11 REQUIRED)
find_package(memwatch REQUIRED)
find_package(bphash REQUIRED)
find_package(libtaskforce REQUIRED)

# For some reason, this has a leading whitespace
string(STRIP "${MPI_CXX_LINK_FLAGS}" MPI_CXX_LINK_FLAGS)

# Modules are created without a prefix
set(CMAKE_SHARED_MODULE_PREFIX "")

message(STATUS "Going to use python libraries: ${PULSAR_CORE_PYTHON_LIBRARIES}")
message(STATUS "Going to use python include dirs: ${PULSAR_CORE_PYTHON_INCLUDE_DIRS}")


#################################################
# Obtain all the sources of the core submodules
#################################################
add_subdirectory(python)
add_subdirectory(output)
add_subdirectory(util)
add_subdirectory(exception)
add_subdirectory(parallel)
add_subdirectory(math)
add_subdirectory(datastore)
add_subdirectory(modulebase)
add_subdirectory(modulemanager)
add_subdirectory(system)
add_subdirectory(testing)




##################################################################################
# MAIN PULSAR MODULE
##################################################################################

####################
# Compilation flags
####################
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
    list(APPEND PULSAR_CORE_CXX_FLAGS "-std=c++11")
    list(APPEND PULSAR_CORE_CXX_FLAGS "-w3")
    list(APPEND PULSAR_CORE_CXX_FLAGS "-restrict;-DRESTRICT=restrict")

    #  2338  : switch statement does not have a default clause
    #list(APPEND PULSAR_CORE_CXX_FLAGS "-ww2338")

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
    list(APPEND PULSAR_CORE_CXX_FLAGS "-wd1418")
    list(APPEND PULSAR_CORE_CXX_FLAGS "-wd1419")
    list(APPEND PULSAR_CORE_CXX_FLAGS "-wd383")
    list(APPEND PULSAR_CORE_CXX_FLAGS "-wd981")
    list(APPEND PULSAR_CORE_CXX_FLAGS "-wd869")
    list(APPEND PULSAR_CORE_CXX_FLAGS "-wd11074")
    list(APPEND PULSAR_CORE_CXX_FLAGS "-wd11076")
    list(APPEND PULSAR_CORE_CXX_FLAGS "-wd444")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    list(APPEND PULSAR_CORE_CXX_FLAGS "-std=c++11")
    list(APPEND PULSAR_CORE_CXX_FLAGS "-DRESTRICT=__restrict__")
    list(APPEND PULSAR_CORE_CXX_FLAGS "-Wall;-Wextra;-pedantic")
    list(APPEND PULSAR_CORE_CXX_FLAGS "-Wfloat-equal;-Wshadow")
    list(APPEND PULSAR_CORE_CXX_FLAGS "-Wswitch-default")
    list(APPEND PULSAR_CORE_CXX_FLAGS "-Wconversion")

    # maybe in the future. Seems to need a very new gcc compiler
    #list(APPEND PULSAR_CORE_CXX_FLAGS "-Wmisleading-indentation")
endif()


###################
# Other flags
###################
list(APPEND PULSAR_CORE_CXX_FLAGS ${OpenMP_CXX_FLAGS})
list(APPEND PULSAR_CORE_CXX_FLAGS ${MPI_CXX_COMPILE_FLAGS})


#################
# Includes
#################
# We need to add the parent of this directory
get_filename_component(PULSAR_CORE_PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)
list(APPEND PULSAR_CORE_CXX_INCLUDES ${PULSAR_CORE_PARENT_DIR})

# External dependencies (mark as system includes)
list(APPEND PULSAR_CORE_CXX_EXTERNAL_INCLUDES ${MEMWATCH_INCLUDE_DIR})
list(APPEND PULSAR_CORE_CXX_EXTERNAL_INCLUDES ${PULSAR_CORE_PYTHON_INCLUDE_DIRS})
list(APPEND PULSAR_CORE_CXX_EXTERNAL_INCLUDES ${MPI_CXX_INCLUDE_PATH})
list(APPEND PULSAR_CORE_CXX_EXTERNAL_INCLUDES ${PYBIND11_INCLUDE_DIR})
list(APPEND PULSAR_CORE_CXX_EXTERNAL_INCLUDES ${EIGEN3_INCLUDE_DIR})
#############################################################
# Libraries and link flags (including interface libraries)
#############################################################
list(APPEND PULSAR_CORE_LINK_FLAGS ${PULSAR_MATH_LIBS})  # Note - not PULSAR_CORE_MATH_LIBS
list(APPEND PULSAR_CORE_LINK_FLAGS ${MEMWATCH_FLAGS})
list(APPEND PULSAR_CORE_LINK_FLAGS bphash)
list(APPEND PULSAR_CORE_LINK_FLAGS libtaskforce)

list(APPEND PULSAR_CORE_LINK_FLAGS ${OpenMP_CXX_FLAGS})
list(APPEND PULSAR_CORE_LINK_FLAGS ${PULSAR_CORE_PYTHON_LIBRARIES})
list(APPEND PULSAR_CORE_LINK_FLAGS ${MPI_CXX_LINK_FLAGS})
list(APPEND PULSAR_CORE_LINK_FLAGS ${MPI_CXX_FLAGS})


###########################
# Fiddle with some rpaths
###########################
list(APPEND PULSAR_CORE_RPATH ${CMAKE_INSTALL_PREFIX}/external/libtaskforce/lib)
list(APPEND PULSAR_CORE_RPATH ${CMAKE_INSTALL_PREFIX}/external/memwatch/lib)


#######################
# Big list of all the files
#######################
function(MakeFullPath FILELIST PREFIX)
   set(LV "")
   foreach(F ${ARGN})
      list(APPEND LV "${PREFIX}/${F}")
   endforeach(F)
   SET(${FILELIST} "${LV}" PARENT_SCOPE)
endfunction(MakeFullPath)

MakeFullPath(PULSAR_DATASTORE_FILES_FULL       "datastore"       ${PULSAR_DATASTORE_FILES})
MakeFullPath(PULSAR_EXCEPTION_FILES_FULL       "exception"       ${PULSAR_EXCEPTION_FILES})
MakeFullPath(PULSAR_MATH_FILES_FULL            "math"            ${PULSAR_MATH_FILES})
MakeFullPath(PULSAR_MODULEBASE_FILES_FULL      "modulebase"      ${PULSAR_MODULEBASE_FILES})
MakeFullPath(PULSAR_MODULEMANAGER_FILES_FULL   "modulemanager"   ${PULSAR_MODULEMANAGER_FILES})
MakeFullPath(PULSAR_OUTPUT_FILES_FULL          "output"          ${PULSAR_OUTPUT_FILES})
MakeFullPath(PULSAR_PARALLEL_FILES_FULL        "parallel"        ${PULSAR_PARALLEL_FILES})
MakeFullPath(PULSAR_PYTHON_FILES_FULL          "python"          ${PULSAR_PYTHON_FILES})
MakeFullPath(PULSAR_SYSTEM_FILES_FULL          "system"          ${PULSAR_SYSTEM_FILES})
MakeFullPath(PULSAR_TESTING_FILES_FULL         "testing"         ${PULSAR_TESTING_FILES})
MakeFullPath(PULSAR_UTIL_FILES_FULL            "util"            ${PULSAR_UTIL_FILES})

list(APPEND PULSAR_CORE_FILE_LIST ${PULSAR_DATASTORE_FILES_FULL})
list(APPEND PULSAR_CORE_FILE_LIST ${PULSAR_EXCEPTION_FILES_FULL})
list(APPEND PULSAR_CORE_FILE_LIST ${PULSAR_MATH_FILES_FULL})
list(APPEND PULSAR_CORE_FILE_LIST ${PULSAR_MODULEBASE_FILES_FULL})
list(APPEND PULSAR_CORE_FILE_LIST ${PULSAR_MODULEMANAGER_FILES_FULL})
list(APPEND PULSAR_CORE_FILE_LIST ${PULSAR_OUTPUT_FILES_FULL})
list(APPEND PULSAR_CORE_FILE_LIST ${PULSAR_PARALLEL_FILES_FULL})
list(APPEND PULSAR_CORE_FILE_LIST ${PULSAR_PYTHON_FILES_FULL})
list(APPEND PULSAR_CORE_FILE_LIST ${PULSAR_SYSTEM_FILES_FULL})
list(APPEND PULSAR_CORE_FILE_LIST ${PULSAR_TENSOR_FILES_FULL})
list(APPEND PULSAR_CORE_FILE_LIST ${PULSAR_TESTING_FILES_FULL})
list(APPEND PULSAR_CORE_FILE_LIST ${PULSAR_UTIL_FILES_FULL})
list(APPEND PULSAR_CORE_FILE_LIST export.cpp)


# Actual module library
add_library(pulsar_core MODULE ${PULSAR_CORE_FILE_LIST})
target_compile_options(pulsar_core PRIVATE ${PULSAR_CORE_CXX_FLAGS})
target_include_directories(pulsar_core PRIVATE ${PULSAR_CORE_CXX_INCLUDES})
target_include_directories(pulsar_core SYSTEM PRIVATE ${PULSAR_CORE_CXX_EXTERNAL_INCLUDES})
target_link_libraries(pulsar_core PRIVATE ${PULSAR_CORE_LINK_FLAGS})

set_target_properties(pulsar_core PROPERTIES INSTALL_RPATH "${PULSAR_CORE_RPATH}")


########################
# Installation of the so
########################
install(TARGETS pulsar_core
        LIBRARY DESTINATION modules/pulsar
        RUNTIME DESTINATION modules/pulsar)




##################################################################
# Make a header-only interface library for pulsar modules to use
##################################################################
# Install the config file
configure_file("pulsarConfig.cmake.in" "pulsarConfig.cmake" @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/pulsarConfig.cmake DESTINATION ${CMAKE_INSTALL_PREFIX})

# Install all python files (for all core modules)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" DESTINATION modules FILES_MATCHING PATTERN "*.py")

# Install all headers (for all core modules)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" DESTINATION include FILES_MATCHING PATTERN "*.hpp")
