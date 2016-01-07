set(BPMODULE_OUTPUT_FILES
            Output.cpp 
            export.cpp
   )


ADD_SUPERMODULE(output 
           "${BPMODULE_OUTPUT_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(output)
