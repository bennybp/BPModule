# Find openmp, boost, and python
find_package(OpenMP)
if(NOT OPENMP_FOUND) 
  message(FATAL_ERROR "Error - OpenMP is required!")
endif()

find_package(MPI)
if(NOT MPI_CXX_FOUND) 
  message(FATAL_ERROR "Error - MPI is required!")
endif()

# For some reason, this has a leading whitespace
string(STRIP "${MPI_CXX_LINK_FLAGS}" MPI_CXX_LINK_FLAGS)

find_package(Boost COMPONENTS python3 REQUIRED)

set(Python_ADDITIONAL_VERSIONS "3.3;3.4")
find_package(PythonLibs REQUIRED)
  

# Don't use anything (ie, lib) as a prefix to SO
set(CMAKE_SHARED_MODULE_PREFIX "")
