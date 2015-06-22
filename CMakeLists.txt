cmake_minimum_required(VERSION 3.0)
project(BPModule CXX)

set(CMAKE_SHARED_MODULE_PREFIX "")

set(BPMODULE_CXX_FLAGS "-std=c++11" "-Wall" "-pedantic -g -O0")
set(BPMODULE_CXX_INCLUDES "${CMAKE_SOURCE_DIR}")
set(BPMODULE_CXX_LINK_FLAGS "")


find_package(OpenMP)
if(OPENMP_FOUND)
  list(APPEND BPMODULE_CXX_FLAGS "${OpenMP_CXX_FLAGS}")
  list(APPEND BPMODULE_CXX_LINK_FLAGS "${OpenMP_CXX_FLAGS}")
else()
  message(FATAL_ERROR "Error - OpenMP is required!")
endif()

add_subdirectory(BPModule)
add_subdirectory(modules)


install(DIRECTORY bppython DESTINATION ${CMAKE_INSTALL_PREFIX})
