cmake_minimum_required(VERSION 3.0)
project(BPModule CXX)
enable_language(C) # for find_package(LAPACK)


#################################
# Load macros
#################################
set(BPMODULE_PATH ${CMAKE_SOURCE_DIR})
include("cmake/Flags.cmake")
include("cmake/Common.cmake")
include("cmake/AddModule.cmake")
include("cmake/Install.cmake")
include("cmake/Tensor.cmake")


#################################
# Add the various subdirectories
#################################
add_subdirectory(bpmodule)


#install(DIRECTORY bppython DESTINATION ${CMAKE_INSTALL_PREFIX}/modules)
install(DIRECTORY cmake DESTINATION ${CMAKE_INSTALL_PREFIX})
