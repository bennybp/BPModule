set(BPMODULE_PARALLEL_FILES
            Parallel.cpp
            InitParallel.cpp
            FinalizeParallel.cpp
            export.cpp
   )
  


ADD_MODULE(bpmodule_parallel
           "${BPMODULE_PARALLEL_FILES}"
           ""
           ""
           "")

INSTALL_MODULE(bpmodule_parallel "")
