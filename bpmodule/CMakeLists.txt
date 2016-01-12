cmake_minimum_required(VERSION 3.0)


#################################
# Load macros
#################################
include("cmake/Flags.cmake")
include("cmake/Python.cmake")
include("cmake/AddCoreModule.cmake")
include("cmake/InstallCoreModule.cmake")


##########################
# Dependencies
##########################
find_package(OpenMP REQUIRED)
find_package(MPI REQUIRED)
find_package(LAPACK REQUIRED)
find_package(Boost REQUIRED)

##########################
# Dependencies in external
##########################
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../external/cmake")
list(APPEND CMAKE_PREFIX_PATH "${DESTDIR}/${CMAKE_INSTALL_PREFIX}/external/pybind11")
list(APPEND CMAKE_PREFIX_PATH "${DESTDIR}/${CMAKE_INSTALL_PREFIX}/external/memwatch")
list(APPEND CMAKE_PREFIX_PATH "${DESTDIR}/${CMAKE_INSTALL_PREFIX}/external/libelemental")
find_package(pybind11 REQUIRED)
find_package(memwatch REQUIRED)
find_package(libelemental REQUIRED)


# For some reason, this has a leading whitespace
string(STRIP "${MPI_CXX_LINK_FLAGS}" MPI_CXX_LINK_FLAGS)




# Modules are created without a prefix
set(CMAKE_SHARED_MODULE_PREFIX "")


add_subdirectory(python)
add_subdirectory(output)
add_subdirectory(util)
add_subdirectory(exception)
add_subdirectory(parallel)
add_subdirectory(math)
add_subdirectory(tensor)
add_subdirectory(datastore)
add_subdirectory(modulebase)
add_subdirectory(modulelocator)
add_subdirectory(molecule)
add_subdirectory(basisset)
add_subdirectory(testing)


######################################
# Make a header-only interface library
######################################
# Install the config file
install(FILES bpmoduleConfig.cmake DESTINATION ${CMAKE_INSTALL_PREFIX})

# Install all python files (for all core modules)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" DESTINATION modules FILES_MATCHING PATTERN "*.py")

# Install all headers (for all core modules)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" DESTINATION include FILES_MATCHING PATTERN "*.h")
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}" DESTINATION include FILES_MATCHING PATTERN "*.hpp")
