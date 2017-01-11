cmake_minimum_required(VERSION 3.2)
project(pulsar-test CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS False)

# CMake doesn't support Intel CXX standard until cmake 3.6
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
  if("${CMAKE_VERSION}" VERSION_LESS "3.6")
      add_compile_options(-std=c++${CMAKE_CXX_STANDARD})
  endif()
endif()

# Find the pulsar package
# This may be in a superbuild staging area
find_package(pulsar CONFIG REQUIRED)

##############################
# Modules used by core tests
##############################
add_subdirectory(testmodules)

########################
# Core pulsar tests
########################

foreach(dir datastore math system modulebase modules util)
  add_subdirectory(${dir})
  install(FILES ${CMAKE_BINARY_DIR}/${dir}/CTestTestfile.cmake DESTINATION ${dir})
endforeach()

# This file was created from the various add_test commands
install(FILES ${CMAKE_BINARY_DIR}/CTestTestfile.cmake DESTINATION .)
