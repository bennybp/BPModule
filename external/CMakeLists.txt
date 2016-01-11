include(ExternalProject)


#####################################
# Projects in the external directory
#####################################


##################
# Memwatch
##################
ExternalProject_Add(memwatch_external
                    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/memwatch
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/external/memwatch -DDESTDIR=${CMAKE_BINARY_DIR}/stage
                    INSTALL_COMMAND make install DESTDIR=${CMAKE_BINARY_DIR}/stage
)


##################
# Pybind11
##################
ExternalProject_Add(pybind11_external
                    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/pybind11
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/external/pybind11 -DPYBIND11_PYTHON_VERSION=${PYTHONLIBS_MAJORMINOR_VERSION_STRING} -DDESTDIR=${CMAKE_BINARY_DIR}/stage
                    INSTALL_COMMAND make install DESTDIR=${CMAKE_BINARY_DIR}/stage
)

#################################################
# Install all external libraries in one command
#################################################
install(DIRECTORY ${CMAKE_BINARY_DIR}/stage/${CMAKE_INSTALL_PREFIX}/ DESTINATION ${CMAKE_INSTALL_PREFIX} USE_SOURCE_PERMISSIONS)

