cmake_minimum_required(VERSION 3.0)

###############################
# Build memwatch
###############################

include(ExternalProject)


# Always build. Assume it's not installed elsewhere  
ExternalProject_Add(memwatch_external_build
                      SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/memwatch-source
                      CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                      INSTALL_COMMAND make install DESTDIR=${DESTDIR}
                     )

set(EXTERNAL_MEMWATCH_INCLUDE_DIR "include")  # Relative to install directory


# Create the import/export library
add_library(memwatch_interface INTERFACE)
target_include_directories(memwatch_interface INTERFACE
       $<INSTALL_INTERFACE:${EXTERNAL_MEMWATCH_INCLUDE_DIR}>
)

find_library(EXTERNAL_MEMWATCH_LIBRARIES memwatch REQUIRED
             PATHS "${DESTDIR}/${CMAKE_INSTALL_PREFIX}"
             PATH_SUFFIXES "lib")

target_link_libraries(memwatch_interface INTERFACE
       $<INSTALL_INTERFACE:\${_IMPORT_PREFIX}/lib/${CMAKE_SHARED_LIBRARY_PREFIX}memwatch${CMAKE_SHARED_LIBRARY_SUFFIX}>
)


# Install stuff and create the export file for use with bpmodule core project
install(TARGETS memwatch_interface EXPORT memwatch)
install(FILES   memwatchConfig.cmake DESTINATION cmake)
install(EXPORT  memwatch NAMESPACE External:: DESTINATION cmake)
