include(CMakeFindDependencyMacro)

# Modules are created without a prefix
set(CMAKE_SHARED_MODULE_PREFIX "")

# Find the dependencies for bpmodule
# Add the install path for bpmodule to the search path
set(BPMODULE_EXTERNAL_DIR "${CMAKE_CURRENT_LIST_DIR}/external")
list(APPEND CMAKE_PREFIX_PATH "${BPMODULE_EXTERNAL_DIR}/pybind11")
list(APPEND CMAKE_PREFIX_PATH "${BPMODULE_EXTERNAL_DIR}/memwatch")
list(APPEND CMAKE_PREFIX_PATH "${BPMODULE_EXTERNAL_DIR}/libelemental")

# And the find macros
list(APPEND CMAKE_MODULE_PATH "${BPMODULE_EXTERNAL_DIR}/cmake")

# Find stuff
set(Python_ADDITIONAL_VERSIONS "3.3;3.4;3.5;3.6")
find_package(PythonLibs REQUIRED)
string(REGEX REPLACE "^([0-9]+)\\.([0-9]+)\\.[0-9]+.*"
                     "\\1.\\2"
                     PYTHONLIBS_MAJORMINOR_VERSION_STRING
                     "${PYTHONLIBS_VERSION_STRING}")


find_package(OpenMP REQUIRED)
find_package(MPI REQUIRED)
find_package(LAPACK REQUIRED)
find_package(Boost REQUIRED)

# Find external projects
find_package(memwatch REQUIRED)
find_package(libelemental REQUIRED)
find_package(pybind11 REQUIRED)

# Create the target to link against
add_library(bpmodule INTERFACE)

# Required options
target_compile_options(bpmodule INTERFACE "-std=c++11")

#######################################
# Add the various include directories
#######################################
# Main bpmodule headers
list(APPEND BPMODULE_CXX_INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include)

# Main dependencies
list(APPEND BPMODULE_CXX_INCLUDES ${Boost_INCLUDE_DIRS})
list(APPEND BPMODULE_CXX_INCLUDES ${PYTHON_INCLUDE_DIRS})
list(APPEND BPMODULE_CXX_INCLUDES ${MPI_CXX_INCLUDE_PATH})

# External dependencies
list(APPEND BPMODULE_CXX_INCLUDES ${PYBIND11_INCLUDE_DIR})
list(APPEND BPMODULE_CXX_INCLUDES ${MEMWATCH_INCLUDE_DIR})
list(APPEND BPMODULE_CXX_INCLUDES ${LIBELEMENTAL_INCLUDE_DIR})
target_include_directories(bpmodule INTERFACE ${BPMODULE_CXX_INCLUDES})



#######################################
# Add the libraries to link to
#######################################

# Sometimes this has a leading whitespace which CMake doesn't like
string(STRIP "${MPI_CXX_LINK_FLAGS}" MPI_CXX_LINK_FLAGS)

# Main dependencies
# Shouldn't really be needed?
#list(APPEND BPMODULE_CXX_LINK_FLAGS ${OpenMP_CXX_FLAGS})
#list(APPEND BPMODULE_CXX_LINK_FLAGS ${Boost_LIBRARIES})
#list(APPEND BPMODULE_CXX_LINK_FLAGS ${PYTHON_LIBRARIES})
#list(APPEND BPMODULE_CXX_LINK_FLAGS ${BLAS_LIBRARIES})
#list(APPEND BPMODULE_CXX_LINK_FLAGS ${LAPACK_LIBRARIES})
list(APPEND BPMODULE_CXX_LINK_FLAGS ${MPI_CXX_LINK_FLAGS})
#list(APPEND BPMODULE_CXX_LINK_FLAGS ${MPI_CXX_LIBRARIES})

# External dependencies
# Shouldn't really be needed?
#list(APPEND BPMODULE_CXX_LINK_FLAGS ${MEMWATCH_LIBRARIES})
#list(APPEND BPMODULE_CXX_LINK_FLAGS ${LIBELEMENTAL_LIBRARIES})

target_link_libraries(bpmodule INTERFACE ${BPMODULE_CXX_LINK_FLAGS})

