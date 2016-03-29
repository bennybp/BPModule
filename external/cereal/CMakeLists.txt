include(ExternalProject)

# Find if it already exists? (it's not required yet)
find_package(cereal QUIET)                                

# If not found, build it
# (Actually, cereal doesn't have a make install, so skip building, etc,
# and just copy the headers
if(NOT PYBIND11_FOUND)
  message(STATUS "We will install cereal for you")
  ExternalProject_Add(cereal_external
                      SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cereal-source
                      CONFIGURE_COMMAND echo "Skipping configuring for header-only library"
                      BUILD_COMMAND echo "Skipping building for header-only library"
                      INSTALL_COMMAND mkdir -p ${CMAKE_BINARY_DIR}/stage/${CMAKE_INSTALL_PREFIX}/external/cereal &&
                                      cp -R ${CMAKE_CURRENT_SOURCE_DIR}/cereal-source/include ${CMAKE_BINARY_DIR}/stage/${CMAKE_INSTALL_PREFIX}/external/cereal
                     )
else()
  # Add a dummy library for dependencies
  add_library(cereal_external INTERFACE)
endif()
