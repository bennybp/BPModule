
# Find openmp, boost, and python
find_package(OpenMP)
find_package(Boost COMPONENTS python3 REQUIRED)

set(Python_ADDITIONAL_VERSIONS "3.3;3.4")
find_package(PythonLibs REQUIRED)
  

# Don't use anything (ie, lib) as a prefix to SO
set(CMAKE_SHARED_MODULE_PREFIX "")
