############################
# Paths and options for
# external dependencies
############################
set(BPMODULE_MPI FALSE CACHE BOOL "Use MPI")
set(BPMODULE_LIBEL_PATH "/usr" CACHE PATH "Path to LibElemental. Should contain include, lib subdirectories")


# Don't use anything (ie, lib) as a prefix to SO
set(CMAKE_SHARED_MODULE_PREFIX "")



####################
# OpenMP is required
####################
find_package(OpenMP)
if(NOT OPENMP_FOUND) 
  message(FATAL_ERROR "Error - OpenMP is required!")
endif()

#######################
# Find MPI if requested
#######################
find_package(MPI)
if(NOT MPI_CXX_FOUND) 
  message(FATAL_ERROR "Cannot find MPI!")
endif()

# For some reason, this has a leading whitespace
string(STRIP "${MPI_CXX_LINK_FLAGS}" MPI_CXX_LINK_FLAGS)



############################
# Find boost (with python3)
############################
find_package(Boost COMPONENTS python3 REQUIRED)

############################
# Find Python libraries
############################
set(Python_ADDITIONAL_VERSIONS "3.3;3.4")
find_package(PythonLibs REQUIRED)


############################
# Math libraries  
############################
find_package(LAPACK) # Also finds blas
if(NOT LAPACK_FOUND) 
  message(FATAL_ERROR "Error - LAPACK is required!")
endif()


############################
# Matrix library
############################
message(STATUS "Using libelemental from ${BPMODULE_LIBEL_PATH}")

