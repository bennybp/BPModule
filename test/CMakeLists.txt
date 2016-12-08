cmake_minimum_required(VERSION 3.0)

#Should find the Pulsar in the staging area
#This sets PULSAR_RUNTEST variable
find_package(pulsar REQUIRED)

include(CTestMacros)

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
