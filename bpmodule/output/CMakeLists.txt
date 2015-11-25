set(BPMODULE_OUTPUT_FILES
            Output.cpp 
            export.cpp
   )

set(BPMODULE_OUTPUT_HEADERS
            Output.hpp
   )

ADD_SUPERMODULE(output 
           "${BPMODULE_OUTPUT_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(output "${BPMODULE_OUTPUT_HEADERS}" "")
