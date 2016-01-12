set(BPMODULE_PARALLEL_FILES
            Parallel.cpp
            export.cpp
   )
  

ADD_COREMODULE(parallel
           "${BPMODULE_PARALLEL_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(parallel)
