cmake_minimum_required(VERSION 3.2)

# Find the pulsar package
# This may be in a superbuild staging area
find_package(pulsar CONFIG REQUIRED)

############################
# C++14 by default
############################
set(CMAKE_CXX_EXTENSIONS False CACHE BOOL "Enable/Disable compiler-specific C++ extensions")
set(CMAKE_CXX_STANDARD 14 CACHE STRING "Which C++ standard to use")

# CMake doesn't support Intel CXX standard until cmake 3.6
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Intel")
  if("${CMAKE_VERSION}" VERSION_LESS "3.6")
      add_definitions(-std=c++${CMAKE_CXX_STANDARD})
  endif()
endif()

##############################
# Modules used by core tests
##############################
add_subdirectory(testmodules)

########################
# Core pulsar tests
########################
foreach(dir datastore math system modulebase modules)
  add_subdirectory(${dir})
  install(FILES ${CMAKE_BINARY_DIR}/${dir}/CTestTestfile.cmake DESTINATION ${dir})
endforeach()

# This file was created from the various add_test commands
install(FILES ${CMAKE_BINARY_DIR}/CTestTestfile.cmake DESTINATION .)
