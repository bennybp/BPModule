set(BPMODULE_PARALLEL_FILES
            Parallel.cpp
            export.cpp
   )
  
set(BPMODULE_PARALLEL_HEADERS
            Parallel.hpp
   )


ADD_MODULE(parallel
           "${BPMODULE_PARALLEL_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(parallel "${BPMODULE_PARALLEL_HEADERS}" "")
