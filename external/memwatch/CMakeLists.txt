include(ExternalProject)


# Always build. Assume it's not installed elsewhere  
ExternalProject_Add(memwatch_external
                    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/memwatch-source
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/external/memwatch
                               -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                               -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                    INSTALL_COMMAND make install DESTDIR=${CMAKE_BINARY_DIR}/stage
                     )

