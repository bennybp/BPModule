cmake_minimum_required(VERSION 3.0)


#################################
# Load macros
#################################
include("cmake/CoreModule.cmake")


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
find_package(Boost REQUIRED)


##########################
# Dependencies in external
##########################
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../external/cmake")
list(APPEND CMAKE_PREFIX_PATH "${DESTDIR}/${CMAKE_INSTALL_PREFIX}/external/pybind11")
list(APPEND CMAKE_PREFIX_PATH "${DESTDIR}/${CMAKE_INSTALL_PREFIX}/external/cereal")
list(APPEND CMAKE_PREFIX_PATH "${DESTDIR}/${CMAKE_INSTALL_PREFIX}/external/memwatch")
list(APPEND CMAKE_PREFIX_PATH "${DESTDIR}/${CMAKE_INSTALL_PREFIX}/external/libtaskforce")
list(APPEND CMAKE_PREFIX_PATH "${DESTDIR}/${CMAKE_INSTALL_PREFIX}/external/libtaskforce/external/madness")
list(APPEND CMAKE_PREFIX_PATH "${DESTDIR}/${CMAKE_INSTALL_PREFIX}/external/tensor")
find_package(pybind11 REQUIRED)
find_package(cereal REQUIRED)
find_package(memwatch REQUIRED)
find_package(libtaskforce REQUIRED)
find_package(tensor REQUIRED)

# For some reason, this has a leading whitespace
string(STRIP "${MPI_CXX_LINK_FLAGS}" MPI_CXX_LINK_FLAGS)




# Modules are created without a prefix
set(CMAKE_SHARED_MODULE_PREFIX "")

message(STATUS "Going to use python libraries: ${PULSAR_CORE_PYTHON_LIBRARIES}")
message(STATUS "Going to use python include dirs: ${PULSAR_CORE_PYTHON_INCLUDE_DIRS}")

add_subdirectory(python)
add_subdirectory(output)
add_subdirectory(util)
add_subdirectory(exception)
add_subdirectory(parallel)
add_subdirectory(math)
add_subdirectory(tensor)
add_subdirectory(datastore)
add_subdirectory(modulebase)
add_subdirectory(modulemanager)
add_subdirectory(system)
add_subdirectory(testing)


######################################
# Make a header-only interface library
######################################
# Install the config file
configure_file("pulsarConfig.cmake.in" "pulsarConfig.cmake" @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/pulsarConfig.cmake DESTINATION ${CMAKE_INSTALL_PREFIX})

# Install all python files (for all core modules)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" DESTINATION modules FILES_MATCHING PATTERN "*.py")

# Install all headers (for all core modules)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" DESTINATION include FILES_MATCHING PATTERN "*.hpp")
