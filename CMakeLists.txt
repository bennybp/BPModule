cmake_minimum_required(VERSION 3.0)
project(BPModule CXX)
enable_language(C)


#################################
# Load macros
#################################
set(BPMODULE_PATH ${CMAKE_SOURCE_DIR})
include("cmake/Flags.cmake")
include("cmake/Common.cmake")
include("cmake/External.cmake")
include("cmake/AddModule.cmake")
include("cmake/Install.cmake")
include("cmake/Tensor.cmake")


#################################
# Add the various subdirectories
#################################
add_subdirectory(memwatch)
add_subdirectory(bpmodule)



#################################
# Installation
#################################
# Install some random stuff
install(DIRECTORY cmake DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY basis DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY test DESTINATION ${CMAKE_INSTALL_PREFIX} USE_SOURCE_PERMISSIONS)

# Install the export file
install(EXPORT bpmodule DESTINATION cmake)
