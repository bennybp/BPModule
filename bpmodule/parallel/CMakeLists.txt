set(BPMODULE_PARALLEL_FILES
            Parallel.cpp
            InitParallel.cpp
            FinalizeParallel.cpp
            export.cpp
   )
  


ADD_MODULE(parallel
           "${BPMODULE_PARALLEL_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(parallel "")
