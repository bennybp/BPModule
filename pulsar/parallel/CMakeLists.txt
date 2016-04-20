set(PULSAR_PARALLEL_FILES
            InitFinalize.cpp
            export.cpp
   )
 
ADD_COREMODULE(parallel
           "${PULSAR_PARALLEL_FILES}"
           ""
           "libtaskforce"
           "")

INSTALL_COREMODULE(parallel)

set_target_properties(parallel PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/external/libtaskforce/lib;${CMAKE_INSTALL_PREFIX}/external/libtaskforce/external/madness/lib")
