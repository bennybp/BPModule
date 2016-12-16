include(ExternalProject)


# Always build. Assume it's not installed elsewhere  
ExternalProject_Add(bpprint_external
                    GIT_REPOSITORY https://github.com/bennybp/BPPrint.git
                    GIT_TAG 321147c18180aaf7b2e4fd726a692599fe778660 
                    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}/external/bpprint
                               -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
                               -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
                               -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
                               -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
                               -DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
                               -DBPPRINT_BUILD_PIC=True
                    INSTALL_COMMAND make install DESTDIR=${CMAKE_BINARY_DIR}/stage
                   )

