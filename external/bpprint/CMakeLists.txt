include(ExternalProject)


# Always build. Assume it's not installed elsewhere  
ExternalProject_Add(bpprint_external
                    GIT_REPOSITORY https://github.com/bennybp/BPPrint.git
                    GIT_TAG 596f1ea0b0200e2202c7674e2058673645e4dde4 
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/external/bpprint
                               -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                               -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                               -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                               -DCMAKE_POSITION_INDEPENDENT_CODE=True
                    INSTALL_COMMAND make install DESTDIR=${CMAKE_BINARY_DIR}/stage
                   )

