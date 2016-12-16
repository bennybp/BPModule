include(ExternalProject)


# Always build. Assume it's not installed elsewhere  
ExternalProject_Add(bphash_external
                    GIT_REPOSITORY https://github.com/bennybp/BPHash.git
                    GIT_TAG 2083b4c74efb2d49d39dac80a85b82a02f153f2d 
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/external/bphash
                               -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                               -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                               -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                               -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
                               -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
                               -DBPHASH_BUILD_PIC=True
                    INSTALL_COMMAND make install DESTDIR=${CMAKE_BINARY_DIR}/stage
                   )

