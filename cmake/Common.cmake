find_package(OpenMP)
if(OPENMP_FOUND)
  list(APPEND BPMODULE_CXX_FLAGS "${OpenMP_CXX_FLAGS}")
  list(APPEND BPMODULE_CXX_LINK_FLAGS "${OpenMP_CXX_FLAGS}")
else()
  message(FATAL_ERROR "Error - OpenMP is required!")
endif()


# Find boost and python
find_package(Boost COMPONENTS python3 REQUIRED)

set(Python_ADDITIONAL_VERSIONS "3.3;3.4")
find_package(PythonLibs REQUIRED)
  

# Don't use anything (ie, lib) as a prefix to SO
set(CMAKE_SHARED_MODULE_PREFIX "")
