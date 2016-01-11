cmake_minimum_required(VERSION 3.0)
project(BPModule CXX)
enable_language(C)


include(ExternalProject)

#################################
# Add external projects
#################################
add_subdirectory(external)



#################################
# Core BPModule
#################################
ExternalProject_Add(bpmodule_core
                    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bpmodule
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                               -DCMAKE_PREFIX_PATH=${CMAKE_BINARY_DIR}/stage/${CMAKE_INSTALL_PREFIX}/external
                               -DBPMODULE_LIBEL_PATH=${BPMODULE_LIBEL_PATH}
                               -DMPI_CXX_COMPILER=${MPI_CXX_COMPILER}
                               -DMPI_C_COMPILER=${MPI_C_COMPILER}
                    INSTALL_COMMAND make install DESTDIR=${CMAKE_BINARY_DIR}/stage
)
add_dependencies(bpmodule_core memwatch_external pybind11_external)


#################################
# Create the main bpmodule export
#################################
add_library(bpmodule INTERFACE)
target_include_directories(bpmodule INTERFACE $<INSTALL_INTERFACE:include>
                                              $<INSTALL_INTERFACE:external/pybind11/include>
                                              $<INSTALL_INTERFACE:external/memwatch/include>
                          )

target_compile_options(bpmodule INTERFACE "-std=c++11") 

install(TARGETS bpmodule EXPORT bpmodule)

#################################
# Installation
#################################

# Install the staging directory
install(DIRECTORY ${CMAKE_BINARY_DIR}/stage/${CMAKE_INSTALL_PREFIX}/ DESTINATION ${CMAKE_INSTALL_PREFIX} USE_SOURCE_PERMISSIONS)

# Install some random stuff
install(DIRECTORY basis DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY test DESTINATION ${CMAKE_INSTALL_PREFIX} USE_SOURCE_PERMISSIONS)

# Install the export file
install(EXPORT bpmodule DESTINATION cmake)
