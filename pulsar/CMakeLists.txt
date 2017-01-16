cmake_minimum_required(VERSION 3.2)
project(pulsar CXX)

##########################
# Options to be passed in
##########################
set(CMAKE_CXX_EXTENSIONS False CACHE BOOL "Enable/Disable compiler-specific C++ extensions")
set(CMAKE_CXX_STANDARD 14 CACHE STRING "Which C++ standard to use")

# CMake doesn't support Intel CXX standard until cmake 3.6
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
  if("${CMAKE_VERSION}" VERSION_LESS "3.6")
      list(APPEND PULSAR_CORE_FLAGS -std=c++${CMAKE_CXX_STANDARD})
  endif()
endif()

# Default to Release if build type not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

##############################################
# Puslar modules are created without a prefix
# (ie, modulename.so, not libmodulename.so)
##############################################
set(CMAKE_SHARED_MODULE_PREFIX "")


#############################
# Custom CMake Find* Modules
#############################
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")


######################################################
# Dependencies
######################################################
# Python (we make sure we have a recent enough version
# here so we can bail early)
set(Python_ADDITIONAL_VERSIONS "3.7;3.6;3.5;3.4;3.3")
find_package(PythonInterp 3.3 REQUIRED)

# Others
list(APPEND PULSAR_CORE_DEPS 
            MPI BerkeleyDB Eigen3
            pybind11 memwatch bphash bpprint libtaskforce)
foreach(pkg ${PULSAR_CORE_DEPS})
    find_package(${pkg} REQUIRED)
endforeach()

# For some reason, this sometimes has a leading whitespace
string(STRIP "${MPI_CXX_LINK_FLAGS}" MPI_CXX_LINK_FLAGS)


###################################################################
# Compilation flags
# Compile with many warnings enabled, but selectively
# disable useless ones
###################################################################
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
    list(APPEND PULSAR_CORE_FLAGS "-w3")

    #  2338  : switch statement does not have a default clause
    #list(APPEND PULSAR_CORE_FLAGS "-ww2338")

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
    list(APPEND PULSAR_CORE_FLAGS "-wd1418")
    list(APPEND PULSAR_CORE_FLAGS "-wd1419")
    list(APPEND PULSAR_CORE_FLAGS "-wd383")
    list(APPEND PULSAR_CORE_FLAGS "-wd981")
    list(APPEND PULSAR_CORE_FLAGS "-wd869")
    list(APPEND PULSAR_CORE_FLAGS "-wd11074")
    list(APPEND PULSAR_CORE_FLAGS "-wd11076")
    list(APPEND PULSAR_CORE_FLAGS "-wd444")
elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    list(APPEND PULSAR_CORE_FLAGS "-Wall;-Wextra;-pedantic")
    list(APPEND PULSAR_CORE_FLAGS "-Wfloat-equal;-Wshadow")
    list(APPEND PULSAR_CORE_FLAGS "-Wswitch-default")
    list(APPEND PULSAR_CORE_FLAGS "-Wconversion")
endif()


#########################################
# Includes, compile flags, and linking
#########################################
# We need to add the parent of this directory
get_filename_component(PULSAR_CORE_PARENT_DIR ${CMAKE_SOURCE_DIR} DIRECTORY)
list(APPEND PULSAR_CORE_INCLUDES ${PULSAR_CORE_PARENT_DIR})

# External dependencies (mark as system includes)
list(APPEND PULSAR_CORE_SYSTEM_INCLUDES
            ${MPI_CXX_INCLUDE_PATH}
            ${EIGEN3_INCLUDE_DIR}
            ${BerkeleyDB_INCLUDE_DIR}
)

# Add the mpi flags to the other flags
list(APPEND PULSAR_CORE_FLAGS ${MPI_CXX_COMPILE_FLAGS})

# Libraries and link flags (including interface libraries)
list(APPEND PULSAR_CORE_LINK_FLAGS
            ${MPI_CXX_LINK_FLAGS}
            ${MPI_CXX_LIBRARIES}
            ${BerkeleyDB_LIBRARIES}
            pybind11::pybind11
            memwatch
            bphash
            bpprint
            libtaskforce
)

# Print out lots of info for the user
message(STATUS "CMake build type:       ${CMAKE_BUILD_TYPE}")
message(STATUS "CMake prefix path:      ${CMAKE_PREFIX_PATH}")
message(STATUS "CMake install rpath:    ${CMAKE_INSTALL_RPATH}")
message(STATUS "MPI compiler:           ${MPI_CXX_COMPILER}")
message(STATUS "MPI compile flags:      ${MPI_CXX_COMPILE_FLAGS}")
message(STATUS "MPI link flags:         ${MPI_CXX_LINK_FLAGS}")
message(STATUS "MPI libraries:          ${MPI_CXX_LIBRARIES}")
message(STATUS "Eigen3 includes:        ${EIGEN3_INCLUDE_DIR}")
message(STATUS "BerkeleyDB includes:    ${BerkeleyDB_INCLUDE_DIRS}")
message(STATUS "BerkeleyDB libraries    ${BerkeleyDB_LIBRARIES}")
message(STATUS "Python executable:      ${PYTHON_EXECUTABLE}")
message(STATUS "Python version:         ${PYTHON_VERSION_STRING}")
message(STATUS "Pybind11 path:          ${pybind11_DIR}")
message(STATUS "LibTaskForce path:      ${libtaskforce_DIR}")
message(STATUS "BPHash path:            ${bphash_DIR}")
message(STATUS "BPPrint path:           ${bpprint_DIR}")
message(STATUS "Memwatch path:          ${memwatch_DIR}")
message(STATUS "Pulsar includes:        ${PULSAR_CORE_INCLUDES}")
message(STATUS "Pulsar compile flags:   ${PULSAR_CORE_FLAGS}")
message(STATUS "Pulsar system includes: ${PULSAR_CORE_SYSTEM_INCLUDES}")
message(STATUS "Pulsar link flags:      ${PULSAR_CORE_LINK_FLAGS}")


#################################################
# Obtain all the sources of the core submodules
#################################################
foreach(sub_dir output util exception parallel math datastore modulebase
                modulemanager system testing)

    # Puts all the files into a list called PULSAR_${SUB_DIR}_FILES
    add_subdirectory(${sub_dir})

    # Make the list a full path and add to the master list
    string(TOUPPER ${sub_dir} SUB_DIR)
    foreach(F ${PULSAR_${SUB_DIR}_FILES})
        list(APPEND PULSAR_CORE_FILE_LIST "${sub_dir}/${F}")
    endforeach()
endforeach()

# Some files in this directory
list(APPEND PULSAR_CORE_FILE_LIST export.cpp)

# Actually create the core library
add_library(pulsar_core MODULE ${PULSAR_CORE_FILE_LIST})
target_compile_options(pulsar_core PRIVATE ${PULSAR_CORE_FLAGS})
target_include_directories(pulsar_core PRIVATE ${PULSAR_CORE_INCLUDES})
target_include_directories(pulsar_core SYSTEM PRIVATE ${PULSAR_CORE_SYSTEM_INCLUDES})
target_link_libraries(pulsar_core PRIVATE ${PULSAR_CORE_LINK_FLAGS})


########################
# Installation of the so
########################
install(TARGETS pulsar_core
        LIBRARY DESTINATION lib/pulsar/modules/pulsar
        RUNTIME DESTINATION lib/pulsar/modules/pulsar)

# The CMake configuration file
configure_file("pulsarConfig.cmake.in" pulsarConfig.cmake @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/pulsarConfig.cmake
        DESTINATION share/cmake/pulsar)

# Install the bin directory (helper scripts, etc)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/"
        DESTINATION bin)

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        DESTINATION lib/pulsar/modules
        FILES_MATCHING
        REGEX ".*\.py$"
        PATTERN bin EXCLUDE)

# Install all headers (for all core modules)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        DESTINATION include
        FILES_MATCHING
        REGEX ".*\.h(pp)?$"
        PATTERN bin EXCLUDE)

# Install helper cmake scripts
install(DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/cmake/"
        DESTINATION share/cmake/pulsar)
