#####################################
# Projects in the external directory
#####################################

############################
# The various Find* modules
############################
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(memwatch)
add_subdirectory(pybind11)
add_subdirectory(libelemental)
add_subdirectory(libtaskforce)
add_subdirectory(tensor)

# Install the cmake macros
install(DIRECTORY cmake DESTINATION ${CMAKE_INSTALL_PREFIX}/external)
