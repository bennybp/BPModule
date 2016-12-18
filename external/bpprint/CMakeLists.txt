include(ExternalProject)


# Always build. Assume it's not installed elsewhere  
ExternalProject_Add(bpprint_external
                    GIT_REPOSITORY https://github.com/bennybp/BPPrint.git
                    GIT_TAG 4d2583dca1f3ede4efeec2ca4eb9c14c1d83313c 
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
                               -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                               -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                               -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                               -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
                               -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
                               -DCMAKE_POSITION_INDEPENDENT_CODE=True
                    INSTALL_COMMAND make install DESTDIR=${CMAKE_BINARY_DIR}/stage
                   )

