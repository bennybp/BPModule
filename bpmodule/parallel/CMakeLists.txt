set(BPMODULE_PARALLEL_FILES
            InitFinalize.cpp
            export.cpp
   )
  

ADD_COREMODULE(parallel
           "${BPMODULE_PARALLEL_FILES}"
           ""
           "MADworld"
           "")

INSTALL_COREMODULE(parallel)

set_target_properties(parallel PROPERTIES INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/external/madness/lib")
