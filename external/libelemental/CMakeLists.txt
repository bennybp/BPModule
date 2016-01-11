cmake_minimum_required(VERSION 3.0)

###############################
# Detect and build libelemental
# if necessary
###############################

include(ExternalProject)

# 1.) Find if it already exists?
#     (it's not required yet)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
find_package(libelemental)


# 2.) If found, use those paths
if(${libelemental_FOUND})
  set(EXTERNAL_LIBELEMENTAL_INCLUDE_DIR "${libelemental_INCLUDE_DIR}")
  set(EXTERNAL_LIBELEMENTAL_LIBRARIES "${libelemental_LIBRARIES}")

# 3.) If not found, build it
else()
  ExternalProject_Add(libelemental_external_build
                      SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libelemental-source
                      CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} 
                                 -DBUILD_KISSFFT:Bool=Off
                                 -DBUILD_METIS:Bool=Off
                                 -DINSTALL_PYTHON_PACKAGE:Bool=No
                                 -DBUILD_SHARED_LIBS:Bool=No
                                 -DCMAKE_CXX_FLAGS="-fPIC"
                      INSTALL_COMMAND make install DESTDIR=${DESTDIR}
                     )

  # And use those paths.
  # Can't use find_package here because that gets run at configure time...
  set(EXTERNAL_LIBELEMENTAL_INCLUDE_DIR "include")  # Relative to install directory
  set(EXTERNAL_LIBELEMENTAL_LIBRARIES "libEl.a;libpmrrr.a")

endif()

# 3.) Create the import/export library
add_library(libelemental_interface INTERFACE)
target_include_directories(libelemental_interface INTERFACE
       $<INSTALL_INTERFACE:${EXTERNAL_LIBELEMENTAL_INCLUDE_DIR}>
)

target_link_libraries(libelemental_interface INTERFACE
       $<INSTALL_INTERFACE:\${_IMPORT_PREFIX}/lib/libEl.a;\${_IMPORT_PREFIX}/lib/libpmrrr.a>)
  



# 4.) Install stuff and create the export file for use with bpmodule core project
install(TARGETS libelemental_interface EXPORT libelemental)

install(FILES   libelementalConfig.cmake DESTINATION cmake)
install(EXPORT  libelemental NAMESPACE External:: DESTINATION cmake)
