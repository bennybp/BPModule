cmake_minimum_required(VERSION 3.0)
project(BPModule CXX)


#################################
# Load macros
#################################
include("cmake/Common.cmake")


#################################
# Add the various subdirectories
#################################
add_subdirectory(BPModule)
add_subdirectory(modules)


install(DIRECTORY bppython DESTINATION ${CMAKE_INSTALL_PREFIX}/modules)
