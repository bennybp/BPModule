##################################################
# Actual library
##################################################

set(BPMODULE_PARALLEL_FILES
            Parallel.cpp
            InitParallel.cpp
            FinalizeParallel.cpp
   )
  


ADD_CORELIB(bpmodule_parallel "${BPMODULE_PARALLEL_FILES}"
                              ""
                              "${Boost_INCLUDE_DIRS};${PYTHON_INCLUDE_DIRS}")

