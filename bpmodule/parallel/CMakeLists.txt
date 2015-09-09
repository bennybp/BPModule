set(BPMODULE_PARALLEL_FILES
            Parallel.cpp
            export.cpp
   )
  


ADD_MODULE(parallel
           "${BPMODULE_PARALLEL_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(parallel "")
