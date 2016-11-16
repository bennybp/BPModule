include(ExternalProject)


# Always build. Assume it's not installed elsewhere  
ExternalProject_Add(bpprint_external
                    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bpprint-source
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/external/bpprint
                               -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                               -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                               -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                               -DBPPRINT_EXTRA_CXX_FLAGS=-fPIC
                    INSTALL_COMMAND make install DESTDIR=${CMAKE_BINARY_DIR}/stage
                   )

