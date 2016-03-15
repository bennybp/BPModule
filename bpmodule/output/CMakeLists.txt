set(BPMODULE_OUTPUT_FILES
            GlobalOutput.cpp
            Output.cpp 
            export.cpp
   )


ADD_COREMODULE(output 
           "${BPMODULE_OUTPUT_FILES}"
           ""
           ""
           "")

INSTALL_COREMODULE(output)
