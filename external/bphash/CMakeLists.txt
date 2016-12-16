include(ExternalProject)


# Always build. Assume it's not installed elsewhere  
ExternalProject_Add(bphash_external
                    GIT_REPOSITORY https://github.com/bennybp/BPHash.git
                    GIT_TAG 3a2849d12ae1103d8e71e385be4c93841895ed52
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/external/bphash
                               -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                               -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                               -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                               -DCMAKE_POSITION_INDEPENDENT_CODE=True
                    INSTALL_COMMAND make install DESTDIR=${CMAKE_BINARY_DIR}/stage
                   )

