#####################################
# Projects in the external directory
#####################################

add_subdirectory(memwatch)
add_subdirectory(pybind11)
add_subdirectory(libelemental)

# Install the cmake macros
install(DIRECTORY cmake DESTINATION ${CMAKE_INSTALL_PREFIX}/external)
