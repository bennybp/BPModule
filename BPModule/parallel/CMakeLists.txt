##################################################
# Actual library
##################################################

set(PARALLEL_FILES
            Parallel.cpp
            InitParallel.cpp
            FinalizeParallel.cpp
   )
  


ADD_CORELIB(bpmodule_parallel "${PARALLEL_FILES}"
                              ""
                              "")

