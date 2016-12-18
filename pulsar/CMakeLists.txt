cmake_minimum_required(VERSION 3.1.3)


##########################
# Options to be passed in
##########################
set(PULSAR_CORE_PYTHON_LIBRARIES "" CACHE STRING "Python libraries to link to")
set(PULSAR_CORE_PYTHON_INCLUDE_DIRS "" CACHE STRING "Include directories for python")

set(CMAKE_CXX_EXTENSIONS False CACHE BOOL "Enable/Disable compiler-specific C++ extensions")
set(CMAKE_CXX_STANDARD 14 CACHE STRING "Which C++ standard to use")

# CMake doesn't support Intel CXX standard until cmake 3.6
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
  if("${CMAKE_VERSION}" VERSION_LESS "3.6")
      list(APPEND PULSAR_CORE_CXX_FLAGS -std=c++${CMAKE_CXX_STANDARD})
  endif()
endif()

##########################
# Dependencies
##########################
find_package(MPI REQUIRED)
find_package(BerkeleyDB REQUIRED)


##########################
# Dependencies in external
##########################
foreach(pkg pybind11 memwatch bphash bpprint libtaskforce Eigen3)
    find_package(${pkg} REQUIRED)
endforeach()

# For some reason, this has a leading whitespace
string(STRIP "${MPI_CXX_LINK_FLAGS}" MPI_CXX_LINK_FLAGS)

# Modules are created without a prefix
set(CMAKE_SHARED_MODULE_PREFIX "")

message(STATUS "Going to use python libraries: ${PULSAR_CORE_PYTHON_LIBRARIES}")
message(STATUS "Going to use python include dirs: ${PULSAR_CORE_PYTHON_INCLUDE_DIRS}")


#################################################
# Obtain all the sources of the core submodules
#################################################
foreach(sub_dir output util exception parallel math datastore modulebase
                modulemanager system testing)
    add_subdirectory(${sub_dir})
endforeach()


##################################################################################
# MAIN PULSAR MODULE
##################################################################################

####################
# Compilation flags
####################
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
    list(APPEND PULSAR_CORE_CXX_FLAGS "-w3")

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
list(APPEND PULSAR_CORE_CXX_FLAGS ${MPI_CXX_COMPILE_FLAGS})


#################
# Includes
#################
# We need to add the parent of this directory
get_filename_component(PULSAR_CORE_PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)
list(APPEND PULSAR_CORE_CXX_INCLUDES ${PULSAR_CORE_PARENT_DIR})

# External dependencies (mark as system includes)
list(APPEND PULSAR_CORE_CXX_EXTERNAL_INCLUDES ${PULSAR_CORE_PYTHON_INCLUDE_DIRS})
list(APPEND PULSAR_CORE_CXX_EXTERNAL_INCLUDES ${MPI_CXX_INCLUDE_PATH})
list(APPEND PULSAR_CORE_CXX_EXTERNAL_INCLUDES ${pybind11_INCLUDE_DIR})
list(APPEND PULSAR_CORE_CXX_EXTERNAL_INCLUDES ${EIGEN3_INCLUDE_DIR})
list(APPEND PULSAR_CORE_CXX_EXTERNAL_INCLUES ${DB_INCLUDE_DIR})

#############################################################
# Libraries and link flags (including interface libraries)
#############################################################
list(APPEND PULSAR_CORE_LINK_FLAGS ${PULSAR_MATH_LIBS})  # Note - not PULSAR_CORE_MATH_LIBS
list(APPEND PULSAR_CORE_LINK_FLAGS memwatch)
list(APPEND PULSAR_CORE_LINK_FLAGS bphash)
list(APPEND PULSAR_CORE_LINK_FLAGS bpprint)
list(APPEND PULSAR_CORE_LINK_FLAGS libtaskforce)

list(APPEND PULSAR_CORE_LINK_FLAGS ${PULSAR_CORE_PYTHON_LIBRARIES})
list(APPEND PULSAR_CORE_LINK_FLAGS ${MPI_CXX_LINK_FLAGS})
list(APPEND PULSAR_CORE_LINK_FLAGS ${MPI_CXX_FLAGS})

list(APPEND PULSAR_CORE_LINK_FLAGS ${DB_LIBRARY})


###########################
# Fiddle with some rpaths
###########################
list(APPEND PULSAR_CORE_RPATH ${CMAKE_INSTALL_PREFIX}/lib)


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

foreach(sub_dir datastore exception math modulebase modulemanager output
                parallel system testing util
)
    string(TOUPPER ${sub_dir} SUB_DIR)
    MakeFullPath(PULSAR_${SUB_DIR}_FILES_FULL ${sub_dir} ${PULSAR_${SUB_DIR}_FILES})
    list(APPEND PULSAR_CORE_FILE_LIST ${PULSAR_${SUB_DIR}_FILES_FULL})
endforeach()

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

#################################################################
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
